cmake_minimum_required(VERSION 2.8)
project(hog)

#include_directories(./src)
#file(GLOB lib_src ./src/*.cpp ./src/*.c)
#set(sift sift)
#add_library(${sift} SHARED ${lib_src})


add_executable(hog_testbed ./main.cpp)
target_link_libraries(hog_testbed ${OpenCV_LIBS})