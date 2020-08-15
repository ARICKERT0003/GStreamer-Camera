cmake_minimum_required(VERSION 3.9.6)
project( GStreamer-Camera
  LANGUAGES CXX
  VERSION   0.1.0
  DESCRIPTION "Control cameras using gstreamer library"
)

#############################
# Options
#############################
option(GSTCAM_STATIC_BUILD             "Build static library" OFF)

# Samples
option(GSTCAM_BUILD_SAMPLES            "Build samples" OFF)
option(GSTCAM_BUILD_SAMPLE_CAMERA      "Build camera sample" ON)
message("BUILD_SAMPLES = ${GSTCAM_BUILD_SAMPLES}")
message("BUILD_SAMPLE_CAMERA = ${GSTCAM_BUILD_SAMPLE_CAMERA}")

# Static or Dynamic Link
if(GSTCAM_STATIC_BUILD)
  set(LINK_TYPE STATIC)
else()
  set(LINK_TYPE SHARED)
endif()

#############################
# Set General Variables 
#############################
# Library Variables
set(LIBRARY_NAME GStreamer-Camera)
set(SH_VER 0)

# Library Output Dir
if(NOT GSTCAM_LIBRARY_OUTPUT_DIR)
  set( GSTCAM_LIBRARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
endif()

# Library Install Dir
if(NOT GSTCAM_LIBRARY_INSTALL_DIR)
  set( GSTCAM_LIBRARY_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/lib)
endif()

# Header Install Dir
if(NOT GSTCAM_HEADER_INSTALL_DIR)
  set( GSTCAM_HEADER_INSTALL_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/install/inc)
endif()

# Extern Library Output
if(NOT GSTCAM_EXTERN_LIBRARY_OUTPUT_DIR)
  set( GSTCAM_EXTERN_LIBRARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/lib)
endif()

# Extern Library Install
if(NOT GSTCAM_EXTERN_LIBRARY_INSTALL_DIR)
  set( GSTCAM_EXTERN_LIBRARY_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/lib)
endif()

# Extern Header Install
if(NOT GSTCAM_EXTERN_HEADER_INSTALL_DIR)
  set( GSTCAM_EXTERN_HEADER_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/inc)
endif()

#############################
# Set Compiler Flags Variable
#############################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Base Compiler Flags
set(GSTCAM_DEBUG_FLAGS 
  -Wall 
  -Wextra
  -Werror
  -W
  -DDEBUG
  -g
)

set(GSTCAM_RELEASE_FLAGS
  -pedantic
  -03
)

#############################
# Set Files and Directories
#############################
set(GSTCAM_SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Sink.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Pipeline.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GStreamerBus.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/MessageHandler.cpp
)

set(GSTCAM_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Sink.h
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Pipeline.h
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/GStreamerBus.h
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/MessageHandler.h
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/Containers.h
)
  
# Include Directory
set(GSTCAM_INC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/inc )

#############################
# Find Libraries
#############################
# OpenCV
find_package(OpenCV REQUIRED)   

#GStreamer
find_package(PkgConfig)        
pkg_check_modules(GST REQUIRED
  gstreamer-1.0>=1.4
  gstreamer-sdp-1.0>=1.4
  gstreamer-video-1.0>=1.4
  gstreamer-app-1.0>=1.4
)

set(GSTCAM_LIBS ${OpenCV_LIBS} ${GST_LIBRARIES} )
list(APPEND GSTCAM_DEBUG_FLAGS ${GST_CFLAGS} )
list(APPEND GSTCAM_RELEASE_FLAGS ${GST_CFLAGS} )
list(APPEND GSTCAM_INC_DIRS ${GST_INCLUDE_DIRS} )

#############################
# Make Target and Set Props
#############################
# Make Library Binary
add_library( ${LIBRARY_NAME} ${LINK_TYPE} ${GSTCAM_SRC_FILES} )

# Set Library Properties
set_target_properties( ${LIBRARY_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER "${GSTCAM_HEADER_FILES}"
  ARCHIVE_OUTPUT_DIRECTORY "${GSTCAM_LIBRARY_OUTPUT_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${GSTCAM_LIBRARY_OUTPUT_DIR}"
)

# Target Compiler Flags
target_compile_options( ${LIBRARY_NAME}
  PRIVATE
  "$<$<CONFIG:Debug>:${GSTCAM_DEBUG_FLAGS}>"
  "$<$<CONFIG:Release>:${GSTCAM_RELEASE_FLAGS}>"
)

#############################
# Link Libraries
#############################
# Target Include Directories
target_include_directories( ${LIBRARY_NAME}
  PUBLIC
  ${GSTCAM_INC_DIRS}
)

# Link External Libraries
target_link_libraries( ${LIBRARY_NAME}
  PUBLIC
  ${GSTCAM_LIBS}
)

#############################
# Set Install Options
#############################
# Install 
install(
  TARGETS ${LIBRARY_NAME}
  ARCHIVE DESTINATION ${GSTCAM_LIBRARY_INSTALL_DIR}
  LIBRARY DESTINATION ${GSTCAM_LIBRARY_INSTALL_DIR}
  PUBLIC_HEADER DESTINATION ${GSTCAM_HEADER_INSTALL_DIR}
)

#############################
# Samples
#############################
if(GSTCAM_BUILD_SAMPLES)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/samples)
endif()

