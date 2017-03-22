cmake_minimum_required(VERSION 2.8)
project(thin)


add_executable(thin_example ./thin_example.cpp)
target_link_libraries(thin_example ${OpenCV_LIBS})
