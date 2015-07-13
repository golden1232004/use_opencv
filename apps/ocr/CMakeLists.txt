cmake_minimum_required (VERSION 2.8)
project (ocr)

option (ENABLE_VIDEO_PROCESS "video frame process for ocr" OFF)
if (ENABLE_VIDEO_PROCESS)
  add_definitions(-DENABLE_VIDEO_PROCESS)
endif ()
option (ENABLE_TEMPLATE_MATCH "enable template match" OFF)
if (ENABLE_TEMPLATE_MATCH)
  add_definitions(-DENABLE_TEMPLATE_MATCH)
endif ()


if (NOT CMAKE_CROSSCOMPILING)
  add_executable(character_segment ./character_segment.cpp)
  target_link_libraries(character_segment ${OpenCV_LIBS})
endif ()