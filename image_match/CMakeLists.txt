project(image-match)

include_directories(./include)
file(GLOB lib_src src/*.cpp src/*.c)
set(target_lib imagematch)
add_library(${target_lib} ${lib_src})
add_executable(image_match_example ./systest/image_match_example.cpp)
target_link_libraries(image_match_example ${target_lib} ${OpenCV_LIBS})