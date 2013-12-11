cmake_minimum_required(VERSION 2.8)

include(${CMAKE_CURRENT_LIST_DIR}/../../../../../build/cmake/Roxlu.cmake)

roxlu_add_addon(Utils)
roxlu_add_addon(Math)
roxlu_add_addon(OpenGL)
roxlu_add_addon(VideoCapture)
roxlu_add_addon(Config)
roxlu_add_addon(Audio)
roxlu_add_addon(Gif)
roxlu_add_addon(Image)

roxlu_app_initialize()
# ---------------------------------------------
roxlu_app_add_source_file(main.cpp)
roxlu_app_add_source_file(photobooth/Webcam.cpp)
roxlu_app_add_source_file(photobooth/Graphics.cpp)
roxlu_app_add_source_file(photobooth/PhotoBooth.cpp)
roxlu_app_add_source_file(photobooth/Effects.cpp)
roxlu_app_add_source_file(photobooth/Effect.cpp)
roxlu_app_add_source_file(photobooth/Sprite.cpp)
roxlu_app_add_source_file(photobooth/Counter.cpp)
roxlu_app_add_source_file(photobooth/Button.cpp)
# ---------------------------------------------
roxlu_app_install()
