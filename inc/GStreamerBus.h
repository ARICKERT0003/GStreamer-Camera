#include <iostream>
#include <gst/gst.h>
#include <gst/app/app.h>
#include "MessageHandler.h"
#include "Containers.h"

class GStreamerBus
{
  public:
  std::shared_ptr< GStreamer::MessageHandler > pMessageHandler;

  void init( GstElement* pipeline );
  void attachCallbacks( GstElement*, GstMessage* );
  void dereference();

  void getState( GStreamer::State& );

  private:
  GstBus* pBus;
  GstMessage* pMessage;
};
