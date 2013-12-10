#ifndef PHOTOBOOTH_EFFECTS_H
#define PHOTOBOOTH_EFFECTS_H

#include <opengl/Includes.h>
#include <photobooth/Effect.h>
#include <map>
#include <vector>

class PhotoBooth;

class Effects {

 public:
  Effects(PhotoBooth& booth);
  ~Effects();
  bool setup();
  void beginGrab(); /* begins grabbing the webcam */
  void endGrab(); /* ends grabbing the webcam */
  void apply(); /* apply the effects */
  void draw(); /* draw the webcam with applied effects */
  void next(); /* use next effect */
  void previous(); /* use previous effect */
  void reset(); /* go back to the default effect */

 private:
  bool setupFBO(); /* sets up the fbo into which we render the webcam */
  bool setupEffects(); /* sets up all the effects */
  bool setupBuffers(); /* sets up the VBO + VAO */
  bool addEffect(Effect* f);

 private:
  PhotoBooth& booth;

 public:
  GLuint cam_fbo; /* used to grab the webcam */
  GLuint cam_depth; /* depth buffer */
  GLuint cam_tex_input; /* will contain the webcam image we grabbed */
  GLuint cam_tex_output; /* first pass effect */

  /* fx vao */
  GLuint fx_vao;
  std::map<int, Effect*> effects;
  std::vector<int> names;
  size_t index; /* index of the current effect */
};

inline bool Effects::addEffect(Effect* f) {

  if(!f->setup()) {
    printf("Error: cannot setup the effect: %d\n", f->name);
    return false;
  }

  effects[f->name] = f;
  names.push_back(f->name);
  return true;
}

inline void Effects::reset() {
  index = 0;
}
                    
#endif
