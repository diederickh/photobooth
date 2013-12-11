#ifndef PHOTOBOOTH_H
#define PHOTOBOOTH_H

extern "C" {
#  include <uv.h>
}
#include <photobooth/Webcam.h>
#include <photobooth/Graphics.h>
#include <photobooth/Effects.h>
#include <photobooth/Sprite.h>
#include <photobooth/Counter.h>
#include <photobooth/Button.h>

#define BOOTH_STATE_NONE 0
#define BOOTH_STATE_PREVIEW 1      /* start the effect, show a preview or example of the current effect */
#define BOOTH_STATE_CAPTURE 2      /* capture and show the current effect */
#define BOOTH_STATE_SAVING 3       /* saving the current effect */

void photobooth_counter_callback(int state, void* user); /* gets called by the counter */
void photobooth_save_thread(void* user);

class PhotoBooth {
 public:
  PhotoBooth();
  ~PhotoBooth();
  bool loadSettings();
  bool setup();
  void update();
  void draw();
  void reset();                    /* reset to default state again */

  void takePicture();              /* this will use the current fx.cam_tex_output and draw it statically */
  void savePicture();              /* save the current picture to disc */
  void onKeyPressed(int key);

 public:
  
  /* configuration */
  int win_w;
  int win_h;
  int cam_w;
  int cam_h;

  /* functionality */
  Graphics gfx;
  Webcam cam;
  Effects fx;
  Counter counter;

  /* saving the grabbed photo */
  Button save_yes;
  Button save_no;
  Sprite saving;
  unsigned char* save_pixels;     /* the pixels we read back from GL */
  int state;
  uv_thread_t save_thread;
  uv_cond_t save_cond;
  uv_mutex_t save_mutex;
  bool must_save;
  bool must_stop;
  uint64_t save_timeout;          /* when we should switch back to the initial state */
  uint64_t save_delay;            /* after x-millis we continue and go back to the initial state */
};

#endif
