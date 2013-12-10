#ifndef PHOTOBOOTH_WEBCAM_H
#define PHOTOBOOTH_WEBCAM_H

#include <videocapture/VideoCapture.h>
#include <opengl/Includes.h>
#include <uv.h>

void frame_callback(void* pixels, size_t nbytes, void* user);

class PhotoBooth;

class Webcam {
 public:
  Webcam(PhotoBooth& booth);
  ~Webcam();
  bool setup();
  bool start();
  bool stop();
  void update();
  void draw();

 private:
  void drawWebcam();

 public:
  PhotoBooth& booth;
  VideoCapture cap;
  Mat4 mm; /* webcam model matrix */

  /* GL objects for drawing the cam */
  GLuint tex_yuv;
  GLuint cam_vao;
  GLuint cam_vbo;
  GLuint cam_tex;

  unsigned char* yuv_pixels;
  uv_mutex_t yuv_mutex;
  bool has_new_frame;
};

#endif
