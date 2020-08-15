# GStreamer-Camera

| Author | Created | 
| ------ | ------- |
| Alex Rickert | 08/15/2020 |

## Table of Contents

  - [Overview](#overview)
  - [Dependencies](#dependencies)
  - [Using the library](#using-the-library)
  - [Samples](#samples)
      - [Camera](#camera)
      - [Sample 2](#sample2)

---

## Overview

C++ interface for GStreamer C API. 

## Dependencies

- C++11
- OpenCV 4.3.0
- GStreamer 1.0
- Computer running Linux distribution

## Using the library

1. Clone GStreamer-Camera to project directory
   
    `git clone https://github.com/ARICKERT0003/Gstreamer-Camera.git`

    or 

    `git clone git@github.com:ARICKERT0003/GStreamer-Camera.git`

2. First time build
   
    1. #### Make Build Directory
      
      `mkdir build && cd build`

    2. #### Build Libraries

      `cmake .. && make install`

    3. #### Build Samples [Optional]
    
       Must be done after all previous steps. Will build all samples in sample
       directory.

      `cmake -DGSTCAM_BUILD_SAMPLES=ON .. && make`

      All individual sample variables are default ON but may be switched OFF.

## Samples

Sample code which demonstrates features of the library are located in the `samples/` directory.

### Available Samples

1. #### Camera:

    1. #### Description

    Loosely demonstrates the following capabilities
    
      * GStreamer pipeline creation from string descriptor
      * Connect and disconnect camera
      * Read image from camera

    2. #### CMake Variable
    
    GSTCAM_BUILD_SAMPLE_CAMERA (Default ON)

