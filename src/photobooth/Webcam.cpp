#include <assert.h>
#include <photobooth/Webcam.h>
#include <photobooth/PhotoBooth.h>

void frame_callback(void* pixels, size_t nbytes, void* user) {
  Webcam* c = static_cast<Webcam*>(user);
  assert(c->yuv_pixels);

  uv_mutex_lock(&c->yuv_mutex);
    memcpy((char*)c->yuv_pixels, (char*)pixels, nbytes);
    c->has_new_frame = true;
  uv_mutex_unlock(&c->yuv_mutex);
}

// ---------------------------------------------------

Webcam::Webcam(PhotoBooth& booth)
  :booth(booth)
  ,cam_vao(0)
  ,cam_vbo(0)
  ,cam_tex(0)
  ,has_new_frame(false)
  ,yuv_pixels(NULL)
{
  stop();
  uv_mutex_init(&yuv_mutex);
}

Webcam::~Webcam() {
  printf("@TODO: Stop capture!.\n");
  uv_mutex_destroy(&yuv_mutex);
}

bool Webcam::setup() {

  // setup video capture.
  VideoCaptureSettings cfg;
  cfg.width = booth.cam_w;
  cfg.height = booth.cam_h;
  cfg.in_pixel_format = VIDEOCAPTURE_FMT_UYVY422;
  cfg.in_codec = VIDEOCAPTURE_FMT_JPEG_OPENML;
  cfg.fps = 30.0f;
  
  //cap.printCapabilities(0);
  if(!cap.openDevice(0, cfg, frame_callback, this)) {
    printf("Cannot setup the webcam.\n");
    return false;
  }

  // create rectangle to draw webcam in.
  glGenVertexArrays(1, &cam_vao);
  glBindVertexArray(cam_vao);
  glGenBuffers(1, &cam_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, cam_vbo);
  glEnableVertexAttribArray(0); // pos;
  glEnableVertexAttribArray(1); // tex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (GLvoid*)12);

  Vertices<VertexPT> verts;
  verts.push_back(VertexPT(Vec3(-0.5f, -0.5f, 0.0f), Vec2(0.0f, 0.0f)));  // bottom left
  verts.push_back(VertexPT(Vec3(-0.5f, 0.5f, 0.0f), Vec2(0.0f, 1.0f)));   // top left
  verts.push_back(VertexPT(Vec3(0.5f, -0.5f, 0.0f), Vec2(1.0f, 0.0f)));   // bottom right
  verts.push_back(VertexPT(Vec3(0.5f, 0.5f, 0.0f), Vec2(1.0f, 1.0f)));    // top right

  glBufferData(GL_ARRAY_BUFFER, verts.getNumBytes(), verts.getPtr(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Webcam texture
  glGenTextures(1, &tex_yuv);
  glBindTexture(GL_TEXTURE_2D, tex_yuv);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cfg.width, cfg.height, 0, GL_RGB_422_APPLE, GL_UNSIGNED_SHORT_8_8_APPLE, 0);
  rx_set_texture_parameters();

  glBindTexture(GL_TEXTURE_2D, 0);
  yuv_pixels = new unsigned char[cfg.width * cfg.height * 2];
  if(!yuv_pixels) {
    printf("Cannot allocate yuv pixel container.\n");
    return false;
  }

  mm.translate(booth.win_w * 0.5, booth.win_h * 0.5, 0.0);
  mm.scale(booth.win_w, booth.win_h, 1);
  mm.rotate(HALF_PI, 0.0f, 0.0f, 1.0f);  
  return true;
}

void Webcam::update() {

  uv_mutex_lock(&yuv_mutex);
  {
    if(has_new_frame) {
      glBindTexture(GL_TEXTURE_2D, tex_yuv);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cap.getWidth(), cap.getHeight(), GL_RGB_422_APPLE, GL_UNSIGNED_SHORT_8_8_APPLE, yuv_pixels);
      has_new_frame = false;    
    }
  }
  uv_mutex_unlock(&yuv_mutex);

  cap.update();
}

void Webcam::draw() {
  drawWebcam();
}

void Webcam::drawWebcam() {
  GLint u_pm = glGetUniformLocation(booth.gfx.prog_yuv, "u_pm");
  GLint u_mm = glGetUniformLocation(booth.gfx.prog_yuv, "u_mm");
  GLint u_tex = glGetUniformLocation(booth.gfx.prog_yuv, "u_tex");
  
  glBindVertexArray(cam_vao);
  glUseProgram(booth.gfx.prog_yuv);
  glUniformMatrix4fv(u_pm, 1, GL_FALSE, booth.gfx.ortho_matrix.getPtr());
  glUniformMatrix4fv(u_mm, 1, GL_FALSE, mm.getPtr());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_yuv);
  glUniform1i(u_tex, 0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool Webcam::start() {
  if(!cap.startCapture()) {
    printf("Cannot start the capture.\n");
    return false;
  }
  return true;
}

bool Webcam::stop() {
  if(!cap.stopCapture()) {
    printf("Cannot stop the capture.\n");
    return false;
  }
  return true;
}
