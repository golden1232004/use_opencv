cmake_minimum_required(VERSION 2.8)
project(softcascade)

include_directories(./include/)

file(GLOB lib_src ./*.cpp ./*.c)

set(target_lib softcascade)
add_library(${target_lib} SHARED ${lib_src} )
target_link_libraries(${target_lib} ${OpenCV_LIBS})
add_executable(opencv_trainSoftcascade ./systest/sft.cpp)
target_link_libraries(opencv_trainSoftcascade ${target_lib} ${OpenCV_LIBS})
