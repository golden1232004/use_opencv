cmake_minimum_required(VERSION 2.8)
project (objDetect_LBP)

add_executable(objectDetection_webcam_LBP ./objectDetection_webcam_LBP.cpp)
target_link_libraries(objectDetection_webcam_LBP ${OpenCV_LIBS})
add_executable(objectDetection_image_LBP ./objectDetection_image_LBP.cpp)
target_link_libraries(objectDetection_image_LBP ${OpenCV_LIBS})

