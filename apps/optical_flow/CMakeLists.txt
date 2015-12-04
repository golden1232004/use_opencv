cmake_minimum_required(VERSION 2.8)
project(optical_flow)

#include_directories(./src)
#file(GLOB lib_src ./src/*.cpp ./src/*.c)
#set(sift sift)
#add_library(${sift} SHARED ${lib_src})


add_executable(pyramid_lucas_kanade ./pyramid_lucas_kanade.cpp)
target_link_libraries(pyramid_lucas_kanade ${OpenCV_LIBS})
add_executable(optical-flow_track ./optical-flow_track.cpp)
target_link_libraries(optical-flow_track ${OpenCV_LIBS})