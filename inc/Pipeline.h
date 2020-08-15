#include <gst/gst.h>
#include <gst/app/app.h>
#include "Sink.h"
#include "GStreamerBus.h"
#include "Containers.h"

class GStreamerPipeline
{
  public:
  GStreamerPipeline();
  int getState();
  void createFromDescription(const std::string&);
  bool readFrame( cv::Mat& frame );
  void start();
  void stop();

  private:
  GstElement* pPipeline;
  GError* pError;

  std::shared_ptr< SinkInterface > pSink;
  std::shared_ptr< GStreamerBus > pBus;

  GStreamer::State _state;
};
