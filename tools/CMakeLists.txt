cmake_minimum_required(VERSION 2.8)
project(tools)



include_directories(./include/)

file(GLOB lib_tool_src src/*.cpp src/*.c)
set(tool_lib tool)   #libtool
add_library(${tool_lib} SHARED ${lib_tool_src})

include_directories(./include_opencv)
file(GLOB lib_tool_opencv_src src_opencv/*.cpp src_opencv/*.c)
set(tool_opencv_lib tool_opencv)
add_library(${tool_opencv_lib} ${lib_tool_opencv_src})  #ToDo use SHARED

#----------------------------------------------------------------------------------

add_executable(generate_samples_tool ./tool/generate_samples.cpp)
target_link_libraries(generate_samples_tool  ${OpenCV_LIBS} ${tool_lib})

add_executable(concatenate_string ./tool/concatenate_string.cpp)
target_link_libraries(generate_samples_tool  ${tool_lib})

add_executable(opencv_createsamples  ./tool/opencv_createsamples.cpp)
set_target_properties(opencv_createsamples PROPERTIES
                      DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
                      OUTPUT_NAME "opencv_createsamples")
target_link_libraries(opencv_createsamples ${OpenCV_LIBS} ${tool_lib} ${tool_opencv_lib})

# -----------------------------------------------------------
#  performance
# -----------------------------------------------------------
if (USE)
add_executable(opencv_performance ./tool/opencv_performance.cpp)
set_target_properties(opencv_performance PROPERTIES
                      DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
                      OUTPUT_NAME "opencv_performance")
target_link_libraries(opencv_performance ${OpenCV_LIBS})
endif()

add_executable(generate_infoFile ./tool/generate_infoFile.cpp)



