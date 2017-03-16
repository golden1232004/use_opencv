cmake_minimum_required(VERSION 2.8)
project(edge_detector)


add_executable(edge_detector ./edge_detector.cpp)
target_link_libraries(edge_detector ${OpenCV_LIBS})
