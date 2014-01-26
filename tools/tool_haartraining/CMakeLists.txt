project(haartraining)


if(WIN32)
  link_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

#link_libraries(${OPENCV_HAARTRAINING_DEPS} opencv_haartraining_engine)

# -----------------------------------------------------------
#  Library
# -----------------------------------------------------------

#add_library(opencv_haartraining_engine STATIC ${cvhaartraining_lib_src})
#target_link_libraries(  ${OpenCV_LIBS})

include_directories(../include/)
message(../include/)
# -----------------------------------------------------------
#  haartraining
# -----------------------------------------------------------
if (USE)
add_executable(opencv_haartraining cvhaartraining.h haartraining.cpp)
set_target_properties(opencv_haartraining PROPERTIES
                      DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
                      OUTPUT_NAME "opencv_haartraining")
target_link_libraries(opencv_haartraining ${OpenCV_LIBS})
endif()
# -----------------------------------------------------------
#  createsamples
# -----------------------------------------------------------

add_executable(opencv_createsamples  createsamples.cpp)
set_target_properties(opencv_createsamples PROPERTIES
                      DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
                      OUTPUT_NAME "opencv_createsamples")
target_link_libraries(opencv_createsamples ${OpenCV_LIBS} ${tool_lib})

# -----------------------------------------------------------
#  performance
# -----------------------------------------------------------
if (USE)
add_executable(opencv_performance performance.cpp)
set_target_properties(opencv_performance PROPERTIES
                      DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
                      OUTPUT_NAME "opencv_performance")
target_link_libraries(opencv_performance ${OpenCV_LIBS})
endif()

add_executable(generate_infoFile generate_infoFile.cpp)
# -----------------------------------------------------------
#  Install part
# -----------------------------------------------------------
if (USE)
if(INSTALL_CREATE_DISTRIB)
  if(BUILD_SHARED_LIBS)
    install(TARGETS opencv_haartraining RUNTIME DESTINATION ${OPENCV_BIN_INSTALL_PATH} CONFIGURATIONS Release COMPONENT main)
    install(TARGETS opencv_createsamples RUNTIME DESTINATION ${OPENCV_BIN_INSTALL_PATH} CONFIGURATIONS Release COMPONENT main)
    install(TARGETS opencv_performance RUNTIME DESTINATION ${OPENCV_BIN_INSTALL_PATH} CONFIGURATIONS Release COMPONENT main)
  endif()
else()
  install(TARGETS opencv_haartraining RUNTIME DESTINATION ${OPENCV_BIN_INSTALL_PATH} COMPONENT main)
  install(TARGETS opencv_createsamples RUNTIME DESTINATION ${OPENCV_BIN_INSTALL_PATH} COMPONENT main)
  install(TARGETS opencv_performance RUNTIME DESTINATION ${OPENCV_BIN_INSTALL_PATH} COMPONENT main)
endif()

if(ENABLE_SOLUTION_FOLDERS)
  set_target_properties(opencv_performance PROPERTIES FOLDER "applications")
  set_target_properties(opencv_createsamples PROPERTIES FOLDER "applications")
  set_target_properties(opencv_haartraining PROPERTIES FOLDER "applications")
  set_target_properties(opencv_haartraining_engine PROPERTIES FOLDER "applications")
endif()

endif()
