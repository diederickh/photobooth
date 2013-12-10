#include <photobooth/Sprite.h>
#include <photobooth/PhotoBooth.h>

Sprite::Sprite(PhotoBooth& booth)
  :booth(booth)
  ,tex(0)
  ,x(0)
  ,y(0)
{
}

Sprite::~Sprite() {
  printf("@TODO - Sprite::~Sprite free resources.\n");
}

bool Sprite::load(std::string filename) {

  if(!img.load(filename, true)) {
    return false;
  }

  if(!img.getWidth() || !img.getHeight()) {
    RX_ERROR("Wrong image size.");
    return false;
  }

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  // get GL fmt
  GLenum fmt = GL_RGB;
  switch(img.getPixelFormat()) {
    case IMG_FMT_RGB24: {
      fmt = GL_RGB;
      break;
    }
    case IMG_FMT_RGBA32: {
      fmt = GL_RGBA;
      break;
    }
    default: { 
      RX_ERROR("Invalid pixel format.\n");
      return false;
    }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, 
               GL_RGBA, img.getWidth(), img.getHeight(), 0, 
               fmt, GL_UNSIGNED_BYTE, img.getPixels());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  w = img.getWidth();
  h = img.getHeight();
  return true;
}


void Sprite::draw() {
  GLint u_pm = glGetUniformLocation(booth.gfx.prog_pt, "u_pm");
  GLint u_mm = glGetUniformLocation(booth.gfx.prog_pt, "u_mm");
  GLint u_tex = glGetUniformLocation(booth.gfx.prog_pt, "u_tex");

  mm.setPosition(x,  y, 0.0f);
  mm.setScale(w, -h, 1.0f);

  glUseProgram(booth.gfx.prog_pt);
  glUniformMatrix4fv(u_pm, 1, GL_FALSE, booth.gfx.ortho_matrix.getPtr());
  glUniformMatrix4fv(u_mm, 1, GL_FALSE, mm.getPtr());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(u_tex, 0);

  glBindVertexArray(booth.gfx.img_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
