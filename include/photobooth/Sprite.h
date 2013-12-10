#ifndef APOLLO_SPRITE_H
#define APOLLO_SPRITE_H

#include <opengl/Includes.h>
#include <image/Image.h>
#include <math/Math.h>

class PhotoBooth;

class Sprite {

 public:
  Sprite(PhotoBooth& booth);
  ~Sprite();
  bool load(std::string filename);
  void draw();
  void setPosition(float x, float y);
  void setSize(float w, float h);
 public:
  Image img;
  PhotoBooth& booth;
  Mat4 mm;   /* model matrix */
  GLuint tex;
  float x;
  float y;
  float w; 
  float h;
};

inline void Sprite::setPosition(float xx, float yy) {
  x = xx;
  y = yy;
}

inline void Sprite::setSize(float ww, float hh) {
  w = ww;
  h = hh;
}

#endif

