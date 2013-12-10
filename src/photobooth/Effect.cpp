#include <photobooth/Effect.h>
#include <photobooth/PhotoBooth.h>

Effect::Effect(PhotoBooth& booth, int name)
  :booth(booth)
  ,name(name)

{
}

Effect::~Effect() {
  printf("Effect::~Effect()\n");
}

// GRAYSCALE
// ------------------------------------------------

GrayscaleEffect::GrayscaleEffect(PhotoBooth& booth)
  :Effect(booth, EFFECT_GRAYSCALE)
{
}

GrayscaleEffect::~GrayscaleEffect() {
  printf("GrayscaleEffect::~GrayscaleEffect()\n");
}

void GrayscaleEffect::apply() {

  // setup fbo.
  glBindFramebuffer(GL_FRAMEBUFFER, booth.fx.cam_fbo);
  glViewport(0, 0, booth.win_w, booth.win_h);

  GLenum drawbuf[] = { GL_COLOR_ATTACHMENT1 } ;
  glDrawBuffers(1, drawbuf);
  
  // enable grayscale shader.
  glUseProgram(booth.gfx.prog_grayscale);

  // set input texture.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, booth.fx.cam_tex_input);
  glUniform1i(glGetUniformLocation(booth.gfx.prog_grayscale, "u_tex"), 0);

  // draw a simple quad
  glBindVertexArray(booth.fx.fx_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool GrayscaleEffect::setup() { return true; }
void GrayscaleEffect::update() { }
void GrayscaleEffect::draw() { }

// SEPIA
// ------------------------------------------------
SepiaEffect::SepiaEffect(PhotoBooth& booth):Effect(booth, EFFECT_SEPIA){}
SepiaEffect::~SepiaEffect() {}
void SepiaEffect::draw() {}
bool SepiaEffect::setup() { return true; }
void SepiaEffect::update() {}

void SepiaEffect::apply() {

 // setup fbo.
  glBindFramebuffer(GL_FRAMEBUFFER, booth.fx.cam_fbo);
  glViewport(0, 0, booth.win_w, booth.win_h);

  GLenum drawbuf[] = { GL_COLOR_ATTACHMENT1 } ;
  glDrawBuffers(1, drawbuf);
  
  // enable grayscale shader.
  glUseProgram(booth.gfx.prog_sepia);

  // set input texture.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, booth.fx.cam_tex_input);
  glUniform1i(glGetUniformLocation(booth.gfx.prog_sepia, "u_tex"), 0);

  // draw a simple quad
  glBindVertexArray(booth.fx.fx_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// MIRROR
// ------------------------------------------------
MirrorEffect::MirrorEffect(PhotoBooth& booth):Effect(booth, EFFECT_MIRROR){}
MirrorEffect::~MirrorEffect() {}
void MirrorEffect::draw() {}
bool MirrorEffect::setup() { return true; }
void MirrorEffect::update() {}

void MirrorEffect::apply() {

 // setup fbo.
  glBindFramebuffer(GL_FRAMEBUFFER, booth.fx.cam_fbo);
  glViewport(0, 0, booth.win_w, booth.win_h);

  GLenum drawbuf[] = { GL_COLOR_ATTACHMENT1 } ;
  glDrawBuffers(1, drawbuf);
  
  // enable grayscale shader.
  glUseProgram(booth.gfx.prog_mirror);

  // set input texture.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, booth.fx.cam_tex_input);
  glUniform1i(glGetUniformLocation(booth.gfx.prog_mirror, "u_tex"), 0);

  // draw a simple quad
  glBindVertexArray(booth.fx.fx_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// BULGE
// ------------------------------------------------
BulgeEffect::BulgeEffect(PhotoBooth& booth):Effect(booth, EFFECT_BULGE){}
BulgeEffect::~BulgeEffect() {}
void BulgeEffect::draw() {}
bool BulgeEffect::setup() { return true; }
void BulgeEffect::update() {}

void BulgeEffect::apply() {

 // setup fbo.
  glBindFramebuffer(GL_FRAMEBUFFER, booth.fx.cam_fbo);
  glViewport(0, 0, booth.win_w, booth.win_h);

  GLenum drawbuf[] = { GL_COLOR_ATTACHMENT1 } ;
  glDrawBuffers(1, drawbuf);
  
  // enable grayscale shader.
  glUseProgram(booth.gfx.prog_bulge);

  // set input texture.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, booth.fx.cam_tex_input);
  glUniform1i(glGetUniformLocation(booth.gfx.prog_bulge, "u_tex"), 0);

  // draw a simple quad
  glBindVertexArray(booth.fx.fx_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
