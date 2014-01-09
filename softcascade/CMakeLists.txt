cmake_minimum_required(VERSION 2.8)
project(softcascade)

include_directories(./include/)

file(GLOB lib_src src/*.cpp src/*.c)

set(target_lib softcascade)
add_library(${target_lib} ${lib_src} )
#target_link_libraries(${target_lib} ${OpenCV_LIBS})
add_executable(softcascade_train ./systest/softcascade_train.cpp)
target_link_libraries(softcascade_train ${target_lib} ${OpenCV_LIBS})

add_executable(softcascade_example ./systest/softcascade_example.cpp)
target_link_libraries(softcascade_example ${target_lib} ${OpenCV_LIBS})

add_executable(test_algorithm ./systest/test_algorithm.cpp)
target_link_libraries(test_algorithm ${target_lib} ${OpenCV_LIBS})
