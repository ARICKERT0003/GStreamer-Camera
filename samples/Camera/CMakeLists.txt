cmake_minimum_required(VERSION 3.9.6)
project( GStreamer-Camera
  LANGUAGES CXX
  VERSION   0.1.0
  DESCRIPTION "Control cameras using gstreamer library"
)

#############################
# Variables for all Exe
#############################
link_directories( ${GSTCAM_LIBRARY_OUTPUT_DIR} )

#############################
# Connect
#############################
add_executable( sample-camera sample-camera.cpp )

set_target_properties(sample-camera PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories( sample-camera PUBLIC ${GSTCAM_INC_DIRS} )
target_link_libraries( sample-camera PUBLIC GStreamer-Camera ${GSTCAM_LIBS} )

#############################
# Cam0
#############################
#add_executable( gst-camera-0 cam0.cpp )
#set_target_properties( gst-camera-0 PROPERTIES 
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
#)
#target_compile_options( gst-camera-0 PRIVATE ${CAM_FLAGS} )   # Target Compiler Flags
#target_include_directories( gst-camera-0 PUBLIC ${CAM_DIRS} )
#target_link_libraries( gst-camera-0 PUBLIC ${CAM_LIBS} )
#
##############################
## Cam1
##############################
## Source Files
#set(CAM1_SRC_FILES 
#  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Sink.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/cam1.cpp 
#)
#
#add_executable( gst-camera-1 ${CAM1_SRC_FILES} )
#set_target_properties( gst-camera-1 PROPERTIES 
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
#)
#target_compile_options( gst-camera-1 PRIVATE ${CAM_FLAGS} )   # Target Compiler Flags
#target_include_directories( gst-camera-1 PUBLIC ${CAM_DIRS} )
#target_link_libraries( gst-camera-1 PUBLIC ${CAM_LIBS} )
#
##############################
## Cam2
##############################
## Source Files
#set(CAM2_SRC_FILES 
#  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Sink.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Pipeline.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/inc/GStreamerBus.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/inc/MessageHandler.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Containers.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/cam2.cpp 
#)
#
#add_executable( gst-camera-2 ${CAM2_SRC_FILES} )
#set_target_properties( gst-camera-2 PROPERTIES 
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
#)
#target_compile_options( gst-camera-2 PRIVATE ${CAM_FLAGS} )   # Target Compiler Flags
#target_include_directories( gst-camera-2 PUBLIC ${CAM_DIRS} )
#target_link_libraries( gst-camera-2 PUBLIC ${CAM_LIBS} )
