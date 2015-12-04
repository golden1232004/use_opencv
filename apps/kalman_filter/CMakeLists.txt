cmake_minimum_required(VERSION 2.8)
project(kalman_filter)

#include_directories(./src)
#file(GLOB lib_src ./src/*.cpp ./src/*.c)
#set(sift sift)
#add_library(${sift} SHARED ${lib_src})


add_executable(kalman_filter ./kalman_filter.cpp)
target_link_libraries(kalman_filter ${OpenCV_LIBS})