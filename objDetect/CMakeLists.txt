cmake_minimum_required(VERSION 2.8)
project (objDetect)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../tools/include)
set(deps tool)  #set libtool.so

add_executable(objectDetection_webcam ./objectDetection_webcam.cpp)
target_link_libraries(objectDetection_webcam ${OpenCV_LIBS})
add_executable(objectDetection_image ./objectDetection_image.cpp)
target_link_libraries(objectDetection_image ${OpenCV_LIBS} ${deps})
add_executable(objectDetection_list ./objectDetection_list.cpp)
target_link_libraries(objectDetection_list  ${OpenCV_LIBS} ${deps})

