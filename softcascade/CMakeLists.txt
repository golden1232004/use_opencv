cmake_minimum_required(VERSION 2.8)
project(softcascade)

include_directories(./include/)

file(GLOB lib_src ./*.cpp ./*.c)

set(target_lib softcascade)
add_library(${target_lib} ${lib_src} )
#target_link_libraries(${target_lib} ${OpenCV_LIBS})
add_executable(softcascade_example ./systest/sft.cpp)
target_link_libraries(softcascade_example ${target_lib} ${OpenCV_LIBS})
