#ifndef PHOTOBOOTH_BUTTON_H
#define PHOTOBOOTH_BUTTON_H

#include <photobooth/Sprite.h>
#include <string>

#define BUTTON_STATE_NONE 0
#define BUTTON_STATE_ON 1
#define BUTTON_STATE_OFF 2

class PhotoBooth;

class Button {
 public:
  Button(PhotoBooth& booth);
  bool load(std::string offFile, std::string onFile);
  void setPosition(float x, float y);
  void setSize(float w, float h);
  void draw();
  bool isOn();
  void setOn();
  void setOff();
 public:
  int state;
  PhotoBooth& booth;
  Sprite on;
  Sprite off;
};

inline bool Button::isOn() {
  return state == BUTTON_STATE_ON;
}

inline void Button::setOn() {
  state = BUTTON_STATE_ON;
}

inline void Button::setOff() {
  state = BUTTON_STATE_OFF;
}

#endif
