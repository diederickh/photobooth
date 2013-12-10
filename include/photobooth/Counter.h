#ifndef PHOTOBOOTH_COUNTER_H
#define PHOTOBOOTH_COUNTER_H

extern "C" {
# include <uv.h>
}

#include <vector>
#include <photobooth/Sprite.h>

#define COUNTER_STATE_NONE 0
#define COUNTER_STATE_STARTED 1
#define COUNTER_STATE_READY 2

class PhotoBooth;

//  ----------------------------------

typedef void(*counter_callback)(int state, void* user);


struct CounterCallback {
  CounterCallback();
  counter_callback cb_counter;
  void* user;
};

//  ----------------------------------

class Counter {
 public:
  Counter(PhotoBooth& booth);
  ~Counter();
  bool setup();
  void setPosition(float x, float y);
  void setSize(float w, float h);
  void start();
  void draw();
  void addCallback(counter_callback cb, void* user);
 private:
  void callback(int state);
 public:
  PhotoBooth& booth;
  std::vector<Sprite*> images;
  std::vector<CounterCallback> callbacks;
  int state;
  size_t index;
  uint64_t timeout;
  uint64_t delay;
};

#endif
