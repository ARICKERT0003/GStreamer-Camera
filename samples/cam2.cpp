#include <unistd.h>
#include <iostream>
#include <atomic>
#include <gst/gst.h>
#include <gst/app/app.h>
#include <opencv2/opencv.hpp>
#include "Pipeline.h"

int main (int argc, char *argv[])
{
  GStreamerPipeline pipeline;

  /* Pipeline description */
  std::string gstPipelineString = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), format=(string)NV12, framerate=(fraction)0/1 ! nvvidconv ! video/x-raw, width=(int)1080, height=(int)810, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true";

  /* 
   * Initialize GStreamer 
   * Create pipeline
   * Create sink interface
   * Create bus interface
   */
  pipeline.createFromDescription( gstPipelineString );
  std::cout << "Pipeline Created\n";

  /* Set pipeline to playing */
  pipeline.start();
  std::cout << "Pipeline Started\n";
 
  // Main loop
  int key = -1;
  cv::Mat frame;
  //GstState* state = nullptr;

  //pipeline.getState();
  // Wait for playing state
  //while( pipeline.getState() != GST_STATE_PLAYING)
  //{ usleep(100000000); }

  //usleep(100000000);
  
  while(key==-1) 
  {
    //g_main_iteration(false);
 
    if( pipeline.readFrame(frame) )
    {
      cv::imshow("Frame", frame);
      key = cv::waitKey(30);
    }
    
    //pipeline.getState();
  }
 
  pipeline.stop();

  return 0;
}
