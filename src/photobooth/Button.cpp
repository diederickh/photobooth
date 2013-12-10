#include <photobooth/Button.h>
#include <photobooth/Sprite.h>

Button::Button(PhotoBooth& booth)
  :booth(booth)
  ,on(booth)
  ,off(booth)
  ,state(BUTTON_STATE_NONE)
{
}

bool Button::load(std::string offFile, std::string onFile) {

  if(!on.load(onFile)) {
    return false;
  }

  if(!off.load(offFile)) {
    return false;
  }

  state = BUTTON_STATE_OFF;
  return true;
}

void Button::setPosition(float x, float y) {
  on.setPosition(x, y);
  off.setPosition(x, y);
}

void Button::setSize(float w, float h) {
  on.setSize(w, h);
  off.setSize(w, h);
}

void Button::draw() {
  if(state == BUTTON_STATE_OFF) {
    off.draw();
  }
  else if(state == BUTTON_STATE_ON) {
    on.draw();
  }
}
