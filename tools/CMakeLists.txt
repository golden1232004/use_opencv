cmake_minimum_required(VERSION 2.8)
project(tools)



include_directories(./include/)
file(GLOB lib_tool_src src/*.cpp src/*.c)
set(target_lib tool)
add_library(${target_lib} ${lib_tool_src})

add_executable(generate_samples_tool ./generate_samples.cpp)
target_link_libraries(generate_samples_tool  ${OpenCV_LIBS} ${target_lib})
