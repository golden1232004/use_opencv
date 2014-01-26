cmake_minimum_required(VERSION 2.8)
project(tools)



include_directories(./include/)
file(GLOB lib_tool_src src/*.cpp src/*.c)
set(tool_lib tool)
add_library(${tool_lib} ${lib_tool_src})

add_executable(generate_samples_tool ./tool/generate_samples.cpp)
target_link_libraries(generate_samples_tool  ${OpenCV_LIBS} ${tool_lib})

add_subdirectory(tool_haartraining)


