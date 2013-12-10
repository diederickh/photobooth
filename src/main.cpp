/*
 
  BASIC GLFW + GLXW WINDOW AND OPENGL SETUP 
  ------------------------------------------
  See https://gist.github.com/roxlu/6698180 for the latest version of the example.
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math/Math.h>
#include <utils/Utils.h>
#include <opengl/Includes.h>
#include <videocapture/VideoCapture.h>
#include <config/Config.h>
#include <photobooth/PhotoBooth.h>

PhotoBooth booth;

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);
 
int main() {

  Log log;
  log.setup("photobooth");
  log.mini();

  glfwSetErrorCallback(error_callback);
 
  if(!glfwInit()) {
    printf("error: cannot setup glfw.\n");
    return false;
  }
 
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* win = NULL;
  win = glfwCreateWindow(384, 683, "Photo Booth", NULL, NULL);
  //win = glfwCreateWindow(768, 1366, "openGL", NULL, NULL);
  //win = glfwCreateWindow(720, 1280, "openGL", NULL, NULL);
  if(!win) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwGetWindowSize(win, &booth.win_w, &booth.win_h);

  glfwSetFramebufferSizeCallback(win, resize_callback);
  glfwSetKeyCallback(win, key_callback);
  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);
 
#if !defined(__APPLE__)
  if(glxwInit() != 0) {
    printf("error: cannot initialize glxw.\n");
    ::exit(EXIT_FAILURE);
  }
#endif

  booth.setup();

  while(!glfwWindowShouldClose(win)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    {
      booth.update();
      booth.draw();
    }
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
 
  glfwTerminate();

  return EXIT_SUCCESS;
}
 
void error_callback(int err, const char* desc) {
  printf("glfw error: %s (%d)\n", desc, err);
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  
  if(action != GLFW_PRESS) {
    return;
  }

  booth.onKeyPressed(key);
 
  switch(key) {
    case GLFW_KEY_LEFT: {
      break;
    }
    case GLFW_KEY_RIGHT: {
      break;
    }
    case GLFW_KEY_SPACE: {

      break;
    }
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    }
  };
  
}
 
void resize_callback(GLFWwindow* window, int width, int height) {
 
}

