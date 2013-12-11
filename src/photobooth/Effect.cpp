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

// NONE
// ------------------------------------------------
NoneEffect::NoneEffect(PhotoBooth& booth):Effect(booth, EFFECT_NONE){}
NoneEffect::~NoneEffect() {}
void NoneEffect::draw() {}
bool NoneEffect::setup() { return true; }
void NoneEffect::update() {}

void NoneEffect::apply() {

 // setup fbo.
  glBindFramebuffer(GL_FRAMEBUFFER, booth.fx.cam_fbo);
  glViewport(0, 0, booth.win_w, booth.win_h);

  GLenum drawbuf[] = { GL_COLOR_ATTACHMENT1 } ;
  glDrawBuffers(1, drawbuf);
  
  // enable grayscale shader.
  glUseProgram(booth.gfx.prog_none);

  // set input texture.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, booth.fx.cam_tex_input);
  glUniform1i(glGetUniformLocation(booth.gfx.prog_none, "u_tex"), 0);

  // draw a simple quad
  glBindVertexArray(booth.fx.fx_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
BulgeEffect::BulgeEffect(PhotoBooth& booth):Effect(booth, EFFECT_BULGE),strength(0.1){}
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
  glUniform1f(glGetUniformLocation(booth.gfx.prog_bulge, "u_strength"), strength);

  // draw a simple quad
  glBindVertexArray(booth.fx.fx_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void BulgeEffect::setStrength(float s) {
  strength = s;
}

// DOT 
// ------------------------------------------------
DotEffect::DotEffect(PhotoBooth& booth):Effect(booth, EFFECT_DOT){}
DotEffect::~DotEffect() {}
void DotEffect::draw() {}
bool DotEffect::setup() { return true; }
void DotEffect::update() {}

void DotEffect::apply() {

 // setup fbo.
  glBindFramebuffer(GL_FRAMEBUFFER, booth.fx.cam_fbo);
  glViewport(0, 0, booth.win_w, booth.win_h);

  GLenum drawbuf[] = { GL_COLOR_ATTACHMENT1 } ;
  glDrawBuffers(1, drawbuf);
  
  // enable grayscale shader.
  glUseProgram(booth.gfx.prog_dots);

  // set input texture.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, booth.fx.cam_tex_input);
  glUniform1i(glGetUniformLocation(booth.gfx.prog_dots, "u_tex"), 0);

  // draw a simple quad
  glBindVertexArray(booth.fx.fx_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
