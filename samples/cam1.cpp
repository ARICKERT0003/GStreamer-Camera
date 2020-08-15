#include <unistd.h>
#include <iostream>
#include <atomic>
#include <gst/gst.h>
#include <gst/app/app.h>
#include <opencv2/opencv.hpp>
#include "Sink.h"

/**
* @brief Bus callback
*  Print important messages
*
* @param bus
* @param message
* @param data
* @return gboolean
*/
static gboolean my_bus_callback(GstBus *bus, GstMessage *message, gpointer data)
{
    // Debug message
    //g_print("Got %s message\n", GST_MESSAGE_TYPE_NAME(message));
    switch(GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_ERROR: {
            GError *err;
            gchar *debug;

            gst_message_parse_error(message, &err, &debug);
            g_print("Error: %s\n", err->message);
            g_error_free(err);
            g_free(debug);
            break;
        }
        case GST_MESSAGE_EOS:
            /* end-of-stream */
            break;
        default:
            /* unhandled message */
            break;
    }
    /* we want to be notified again the next time there is a message
    * on the bus, so returning TRUE (FALSE means we want to stop watching
    * for messages on the bus and our callback should not be called again)
    */
    return true;
}

int main (int argc, char *argv[])
{
  GstElement *pipeline;
  SinkInterface sinkInterface;

  /* Initialize GStreamer */
  // Can pass NULL pointer instead
  gst_init (&argc, &argv);

  /* Build the pipeline */
  std::string gstPipelineString = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), format=(string)NV12, framerate=(fraction)0/1 ! nvvidconv ! video/x-raw, width=(int)1080, height=(int)810, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true";

  pipeline = gst_parse_launch(gstPipelineString.c_str(), NULL);

  // Get sink element from pipeline
  GstElement* sink = gst_bin_get_by_name( GST_BIN(pipeline), "sink");

  if(sink == NULL)
  { std::cout << "Error:\tgst_bin_get_by_name\n"; }

  // Initialize sink interface
  sinkInterface.init(sink);
  std::cout << "Initialized\n";

  // Declare bus
  GstBus *bus;
  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  gst_bus_add_watch(bus, my_bus_callback, nullptr);
  gst_object_unref(bus);
  std::cout << "Bus\n";
 
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  std::cout << "State Setting\n";
 
  // Main loop
  int key = -1;
  cv::Mat frame;
  GstState* state = nullptr;

  // Wait for playing state
  //while( (*state) != GST_STATE_PLAYING)
  //{ gst_element_get_state( GST_ELEMENT(pipeline), state, nullptr, 1000000); }
  std::cout << "State Change\n";

  while(key==-1) 
  {
    //g_main_iteration(false);
 
    if( sinkInterface.readFrame(frame) )
    {
      cv::imshow("Frame", frame);
      key = cv::waitKey(30);
    }

    //usleep(10000000);
    //key=1;
  }
 
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
  gst_object_unref(GST_OBJECT(pipeline));
  return 0;
}
