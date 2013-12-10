#include <photobooth/Effects.h>
#include <photobooth/PhotoBooth.h>

Effects::Effects(PhotoBooth& booth)
  :booth(booth)
  ,cam_fbo(0)
  ,cam_depth(0)
  ,cam_tex_input(0)
  ,cam_tex_output(0)
  ,fx_vao(0)
  ,index(0) 
{

}

Effects::~Effects() {
  printf("@TODO Effects::~Effects - cleanup GL.\n");
}

bool Effects::setup() {

  if(!setupFBO()) {
    printf("Error while trying to setup the fbo for the effects.\n");
    return false;
  }

  if(!setupEffects()) {
    return false;
  }

  if(!setupBuffers()) {
    return false;
  }

  return true;

}

bool Effects::setupEffects() {

  if(!addEffect(new GrayscaleEffect(booth))) {
    return false;
  }

  if(!addEffect(new SepiaEffect(booth))) {
    return false;
  }

  if(!addEffect(new MirrorEffect(booth))) {
    return false;
  }

  if(!addEffect(new BulgeEffect(booth))) {
    return false;
  }

  return true;
}

bool Effects::setupBuffers() {
  glGenVertexArrays(1, &fx_vao);
  glBindVertexArray(fx_vao);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return true;
}

bool Effects::setupFBO() {
  
  glGenFramebuffers(1, &cam_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, cam_fbo);

  // depth
  glGenRenderbuffers(1, &cam_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, cam_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, booth.win_w, booth.win_h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, cam_depth);

  // tex
  glGenTextures(1, &cam_tex_input);
  glBindTexture(GL_TEXTURE_2D, cam_tex_input);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, booth.win_w, booth.win_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cam_tex_input, 0);

  glGenTextures(1, &cam_tex_output);
  glBindTexture(GL_TEXTURE_2D, cam_tex_output);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, booth.win_w, booth.win_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, cam_tex_output, 0);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(status != GL_FRAMEBUFFER_COMPLETE) {
    printf("Framebuffer not complete in Effects.\n");
    ::exit(EXIT_FAILURE);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
}

void Effects::beginGrab() {
  glBindFramebuffer(GL_FRAMEBUFFER, cam_fbo);
  glViewport(0, 0, booth.win_w, booth.win_h);

  GLenum drawbufs[] = { GL_COLOR_ATTACHMENT0 } ;
  glDrawBuffers(1, drawbufs);
}

void Effects::endGrab() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, booth.win_w, booth.win_h);
}

void Effects::apply() {
  effects[names[index]]->apply();
}

void Effects::draw() {
  glViewport(0, 0, booth.win_w, booth.win_h);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  GLenum drawbuf[] = { GL_BACK_LEFT } ;
  glDrawBuffers(1, drawbuf);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, cam_fbo);
  glReadBuffer(GL_COLOR_ATTACHMENT1);
  glBlitFramebuffer(0, 0, booth.win_w, booth.win_h,
                    0, 0, booth.win_w, booth.win_h,
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);
  
}



void Effects::previous() {
  if(index == 0) {
    return;
  }
  index--;
}

void Effects::next() {
  if(index >= names.size()-1) {
    return;
  }
  index++;
}
