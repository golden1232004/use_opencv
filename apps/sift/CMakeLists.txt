cmake_minimum_required(VERSION 2.8)
project(sift)

include_directories(./src)
file(GLOB lib_src ./src/*.cpp ./src/*.c)
set(sift sift)
add_library(${sift} SHARED ${lib_src})


add_executable(sift_testbed ./systest/sift_testbed.cpp)
target_link_libraries(sift_testbed ${sift} ${OpenCV_LIBS})