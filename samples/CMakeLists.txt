cmake_minimum_required(VERSION 3.9.6)
project( GStreamer-Camera
  LANGUAGES CXX
  VERSION   0.1.0
  DESCRIPTION "Control cameras using gstreamer library"
)

#############################
# Set General Variables
#############################
# Release or Debug
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS 
#  PRIVATE
#  "$<$<CONFIG:Debug>:${IMGPROC_DEBUG_FLAGS}>"
#  "$<$<CONFIG:Release>:${IMGPROC_RELEASE_FLAGS}>"
#)

#############################
# Recurse into Subdirectories 
#############################
if(GSTCAM_BUILD_SAMPLES)
 
  # Camera
  if(GSTCAM_BUILD_SAMPLE_CAMERA)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Camera)
  endif()

endif()
