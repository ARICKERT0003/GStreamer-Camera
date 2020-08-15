#include <gst/gst.h>
#include <gst/app/app.h>
#include <opencv2/opencv.hpp>

class SinkInterface
{
  public:

  // GStreamer 
  GstAppSinkCallbacks callbacks;
  GstSample* sample;
  GstCaps* caps;
  GstBuffer* buffer;
  GstStructure* structure;
  GstMapInfo map;
  GstElement* sink;
  int width, height;

  // OpenCV
  std::mutex mu;
  cv::Mat frame;

  /**
   *  @fn    init
   *  @brief Sets sink and attached callback functions
   */
  void init(GstElement*);

  /**
   *  @fn    init
   *  @brief Sets sink and attached callback functions
   */
  void initFromPipe(GstElement*);

  void deref();

  GstFlowReturn prerollCallback();

  /**
   *  @fn    bufferCallback
   *  @brief Callback function for when a new buffer is available at sink
   */
  GstFlowReturn bufferCallback( GstAppSink* );


  bool readFrame( cv::Mat& aframe);
};
