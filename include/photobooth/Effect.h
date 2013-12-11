#ifndef WEBCAM_EFFECT_H
#define WEBCAM_EFFECT_H

#include <opengl/Headers.h>

class PhotoBooth;

#define EFFECT_NONE 0
#define EFFECT_GRAYSCALE 1
#define EFFECT_SEPIA 2
#define EFFECT_MIRROR 3
#define EFFECT_BULGE 4
#define EFFECT_PINCH 5
#define EFFECT_DOT 6

// ------------------------------------------------

class Effect {
 public:
  Effect(PhotoBooth& booth, int name);
  virtual ~Effect() = 0;

  virtual bool setup() = 0;
  virtual void update() = 0;
  virtual void apply() = 0;
  virtual void draw() = 0;

 public:
  //  void setReadAndDrawBuffers(GLuint src, GLuint dest);

 public:
  int name;
  PhotoBooth& booth;

};

// ------------------------------------------------

class NoneEffect : public Effect {
 public:
  NoneEffect(PhotoBooth& booth);
  ~NoneEffect();
  bool setup();
  void update();
  void apply();
  void draw();
};


// ------------------------------------------------

class GrayscaleEffect : public Effect {
 public:
  GrayscaleEffect(PhotoBooth& booth);
  ~GrayscaleEffect();
  bool setup();
  void update();
  void apply();
  void draw();
};

// ------------------------------------------------

class SepiaEffect : public Effect {
 public:
  SepiaEffect(PhotoBooth& booth);
  ~SepiaEffect();
  bool setup();
  void update();
  void apply();
  void draw();
};

// ------------------------------------------------

class MirrorEffect : public Effect {
 public:
  MirrorEffect(PhotoBooth& booth);
  ~MirrorEffect();
  bool setup();
  void update();
  void apply();
  void draw();
};

// ------------------------------------------------

class BulgeEffect : public Effect {
 public:
  BulgeEffect(PhotoBooth& booth);
  ~BulgeEffect();
  bool setup();
  void update();
  void apply();
  void draw();
  void setStrength(float s);
 public:
  float strength;
};

// ------------------------------------------------

class DotEffect : public Effect {
 public:
  DotEffect(PhotoBooth& booth);
  ~DotEffect();
  bool setup();
  void update();
  void apply();
  void draw();
};



#endif
