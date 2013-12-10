#include <assert.h>
#include <photobooth/Counter.h>
#include <photobooth/PhotoBooth.h>

//  ----------------------------------

CounterCallback::CounterCallback()
  :cb_counter(NULL) 
  ,user(NULL)
{
}

//  ----------------------------------

Counter::Counter(PhotoBooth& booth)
  :booth(booth)
  ,state(COUNTER_STATE_NONE)
  ,index(0)
  ,delay(1000000LLU * 1000LLU * 1LLU)
{
}

Counter::~Counter() {
}

bool Counter::setup() {

  std::vector<std::string> files;
  files.push_back("1.png");
  files.push_back("2.png");
  files.push_back("3.png");

  for(size_t i = 0; i < files.size(); ++i) {
    Sprite* s = new Sprite(booth);
    if(!s->load(files[i])) {
      return false;
    }
    images.push_back(s);
  }

  return true;
}

void Counter::start() {

  if(state == COUNTER_STATE_STARTED) {
    printf("Already started the counter!\n");
    ::exit(EXIT_FAILURE);
  }

  state = COUNTER_STATE_STARTED;
  timeout = uv_hrtime() + delay;
  index = 0;
}

void Counter::draw() {

  if(state == COUNTER_STATE_NONE ||
     state == COUNTER_STATE_READY) 
    {
      return ;
    }

  uint64_t now = uv_hrtime();

  if(now > timeout) {

    index++;
    timeout = uv_hrtime() + delay;

    if(index >= images.size()) {
      state = COUNTER_STATE_READY;
      callback(state);
      return;
    }
  }

  images[images.size()-(index+1)]->draw();
}


void Counter::setSize(float w, float h) {
  for(size_t i = 0; i < images.size(); ++i) {
    images[i]->setSize(w, h);
  }
}

void Counter::setPosition(float x, float y) {
  for(size_t i = 0; i < images.size(); ++i) {
    images[i]->setPosition(x, y);
  }
}

void Counter::addCallback(counter_callback cb, void* user) {
  CounterCallback call;
  call.cb_counter = cb;
  call.user = user;
  callbacks.push_back(call);
}

void Counter::callback(int state) {
  for(size_t i = 0; i < callbacks.size(); ++i) {
    assert(callbacks[i].cb_counter != NULL);
    callbacks[i].cb_counter(state, callbacks[i].user);
  }
}
