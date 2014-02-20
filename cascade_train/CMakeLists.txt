project(traincascade)

set(traincascade_files traincascade.cpp
  cascadeclassifier.cpp cascadeclassifier.h
  boost.cpp boost.h features.cpp traincascade_features.h
  haarfeatures.cpp haarfeatures.h
  lbpfeatures.cpp lbpfeatures.h
  HOGfeatures.cpp HOGfeatures.h
  imagestorage.cpp imagestorage.h)

set(the_target opencv_traincascade)
add_executable(${the_target} ${traincascade_files})
target_link_libraries(${the_target} ${OpenCV_LIBS} )

set_target_properties(${the_target} PROPERTIES
                      DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
                      ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                      RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
                      INSTALL_NAME_DIR lib
                      OUTPUT_NAME "opencv_traincascade")
