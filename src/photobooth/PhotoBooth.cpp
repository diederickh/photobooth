#include <stdint.h>
#include <photobooth/PhotoBooth.h>
#include <config/Config.h>

// -------------------------------------

void photobooth_save_thread(void* user) {

  bool must_stop = false;
  bool must_save = false;
  PhotoBooth* booth = static_cast<PhotoBooth*>(user);
  unsigned char* flipped_pixels = new unsigned char[booth->win_w * booth->win_h * 3];

  while(true) {

    // check if we need to save something
    uv_mutex_lock(&booth->save_mutex);
    {
      while(booth->must_save == false) {
        uv_cond_wait(&booth->save_cond, &booth->save_mutex);
      }
      must_save = booth->must_save;
      must_stop = booth->must_stop;
      booth->must_save = false;
    }
    uv_mutex_unlock(&booth->save_mutex);

    if(must_stop) {
       break;
    }

    if(!must_save) { /* handle spurious threads */
      continue;
    }

    for(int j = 0; j < booth->win_h; ++j) {
      unsigned char* src = booth->save_pixels + (booth->win_h - j) * (booth->win_w * 3);
      unsigned char* dest = flipped_pixels + j * booth->win_w * 3;
      memcpy(dest, src, booth->win_w * 3);
    }

    std::string filename = "photos/" +rx_get_date_time_string() +".png";
    PNG png;
    png.setPixels(flipped_pixels, booth->win_w, booth->win_h, PNG_COLOR_TYPE_RGB);
    png.save(filename, true);

    // save!
    must_stop = false;
    must_save = false;
  }

  delete[] flipped_pixels;
  flipped_pixels = NULL;
}

// -------------------------------------

void photobooth_counter_callback(int state, void* user) {
  PhotoBooth* booth = static_cast<PhotoBooth*>(user);
  booth->takePicture();
}

// -------------------------------------

PhotoBooth::PhotoBooth()
  :win_w(1280)
  ,win_h(720)
  ,cam_w(1280)
  ,cam_h(720)
  ,gfx(*this)
  ,cam(*this)
  ,fx(*this)
  ,counter(*this)
  ,state(BOOTH_STATE_NONE)
  ,save_yes(*this)
  ,save_no(*this)
  ,saving(*this)
  ,save_pixels(NULL)
  ,must_stop(false)
{
  uv_mutex_init(&save_mutex);
  uv_cond_init(&save_cond);
}

PhotoBooth::~PhotoBooth() {

  uv_mutex_lock(&save_mutex);
    must_stop = true;
    must_save = true; /* must set to true else the thread won't handle the data */
    uv_cond_signal(&save_cond);
  uv_mutex_unlock(&save_mutex);

  uv_thread_join(&save_thread);
  
  uv_mutex_destroy(&save_mutex);
  uv_cond_destroy(&save_cond);

  delete[] save_pixels;
  save_pixels = NULL;
}

bool PhotoBooth::loadSettings() {
  return true;
}

bool PhotoBooth::setup() {

  if(!gfx.setup()) {
    return false;
  }

  if(!cam.setup()) {
    return false;
  }

  if(!fx.setup()) {
    return false;
  }

  if(!counter.setup()) {
    return false;
  }

  if(!saving.load("saving.png")) {
    return false;
  }

  if(!save_yes.load("yes.png", "yes_selected.png")) {
    return false;
  }

  if(!save_no.load("no.png", "no_selected.png")) {
    return false;
  }

  save_pixels = new unsigned char[win_w * win_h * 3];
  if(!save_pixels) {
    RX_ERROR("Cannot allocate the buffer for the save pixels.");
    return false;
  }

  saving.setPosition(win_w * 0.5, win_h * 0.5);
  save_no.setPosition(125.0f, 64.0f);
  save_yes.setPosition(300.0f, 64.0f);

  save_no.setSize(128.0f, 128.0f);
  save_yes.setSize(128.0f, 128.0f);
  saving.setSize(256.0f, 256.0f);

  save_delay = 1000000 * 1000LLU;

  counter.setSize(win_w * 0.25, win_w * 0.25);
  counter.setPosition(win_w * 0.5, win_h * 0.5);
  counter.addCallback(photobooth_counter_callback, this);

  cam.start();

  state = BOOTH_STATE_PREVIEW;

  uv_thread_create(&save_thread, photobooth_save_thread, this);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  return true;
}

void PhotoBooth::update() {
  cam.update();

  if(state == BOOTH_STATE_SAVING) {

    // when saving go back to the initial state after x-millis
    uint64_t now = uv_hrtime();
    if(now >= save_timeout) {
      reset();
    }
  }

}

void PhotoBooth::reset() {
  state = BOOTH_STATE_PREVIEW;
  save_timeout = 0;
  save_yes.setOff();
  save_no.setOff();
  fx.reset();
}

void PhotoBooth::draw() {

#if 0
  cam.draw();
#else

  if(state == BOOTH_STATE_PREVIEW) {
    fx.beginGrab();
       cam.draw();
    fx.endGrab();
    fx.apply();
    fx.draw();

    counter.draw();
  }
  else if(state == BOOTH_STATE_CAPTURE) {
    fx.draw();
    save_yes.draw();
    save_no.draw();
  }
  else if(state == BOOTH_STATE_SAVING) {
    fx.draw();
    saving.draw();
  }

#endif
}

void PhotoBooth::onKeyPressed(int key) {

  if(state == BOOTH_STATE_PREVIEW) {
    if(key == GLFW_KEY_SPACE) {
      counter.start();
    }
    else if(key == GLFW_KEY_LEFT) {
      fx.previous();
    }
    else if(key == GLFW_KEY_RIGHT) {
      fx.next();
    }
  }
  else if(state == BOOTH_STATE_CAPTURE) {
    if(key == GLFW_KEY_Y) {
      state = BOOTH_STATE_PREVIEW;
    }
    else if(key == GLFW_KEY_N) { 
      state = BOOTH_STATE_PREVIEW;
    }
    else if(key == GLFW_KEY_LEFT) {
      save_no.setOn();
      save_yes.setOff();
    }
    else if(key == GLFW_KEY_RIGHT) {
      save_no.setOff();
      save_yes.setOn();
    }
    else if(key == GLFW_KEY_SPACE) {
      if(save_yes.isOn()) {
        savePicture();
        state = BOOTH_STATE_SAVING;
      }
      else {
        reset();
      }
    }
  }
  
}

void PhotoBooth::takePicture() {

  if(state != BOOTH_STATE_PREVIEW) {
    RX_ERROR("Trying to take a picture but we're not previewing an effect");
    ::exit(EXIT_FAILURE);
  }

  state = BOOTH_STATE_CAPTURE;
}

void PhotoBooth::savePicture() {
#if !defined(NDEBUG)
  uint64_t n = uv_hrtime();
#endif

  glReadPixels(0, 0, win_w, win_h, GL_RGB, GL_UNSIGNED_BYTE, save_pixels);
  
  uv_mutex_lock(&save_mutex);
    must_save = true;
    uv_cond_signal(&save_cond);
  uv_mutex_unlock(&save_mutex);

#if !defined(NDEBUG)
  RX_VERBOSE("Took: %llu", (uv_hrtime() - n)/1000000LLU);
#endif

  save_timeout = uv_hrtime() + save_delay;
}
