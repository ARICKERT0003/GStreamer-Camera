#include "Sink.h"

static GstFlowReturn new_preroll( GstAppSink* sink, gpointer sinkInterface)
{ 
  GstFlowReturn status = ((SinkInterface*)sinkInterface)->prerollCallback();
  return status; 
}

static GstFlowReturn new_buffer( GstAppSink* sink, gpointer sinkInterface)
{
  GstFlowReturn status = ((SinkInterface*)sinkInterface)->bufferCallback(sink);
  return status; 
}

void SinkInterface::initFromPipe(GstElement* pipeline)
{
  // Get sink element from pipeline
  sink = gst_bin_get_by_name( GST_BIN(pipeline), "sink");

  if(sink == NULL )
  { 
    throw 1001; 
    return;
  }
  
  // Emit signals (needed?) 
  gst_app_sink_set_emit_signals((GstAppSink*)sink, true);
  
  // Drop old frames once queue is full
  gst_app_sink_set_drop((GstAppSink*)sink, true);

  // Set queue size
  gst_app_sink_set_max_buffers((GstAppSink*)sink, 1);

  // Pack callbacks together 
  callbacks.eos = nullptr; 
  callbacks.new_preroll = new_preroll; 
  callbacks.new_sample = new_buffer;

  // Attach callbacks to sink
  gst_app_sink_set_callbacks(GST_APP_SINK(sink), &callbacks, this, nullptr);
}

void SinkInterface::deref()
{ 
  gst_object_unref( sink ); 
  sink = NULL;
}

void SinkInterface::init(GstElement* sink)
{
  // Emit signals (needed?) 
  gst_app_sink_set_emit_signals((GstAppSink*)sink, true);
  
  // Drop old frames once queue is full
  gst_app_sink_set_drop((GstAppSink*)sink, true);

  // Set queue size
  gst_app_sink_set_max_buffers((GstAppSink*)sink, 1);

  // Pack callbacks together 
  callbacks.eos = nullptr; 
  callbacks.new_preroll = new_preroll; 
  callbacks.new_sample = new_buffer;

  // Attach callbacks to sink
  gst_app_sink_set_callbacks(GST_APP_SINK(sink), &callbacks, this, nullptr);
}

GstFlowReturn SinkInterface::prerollCallback()
{ return GST_FLOW_OK; }

GstFlowReturn SinkInterface::bufferCallback( GstAppSink* appsink )
{
  // Pull sample from sink
  sample = gst_app_sink_pull_sample(appsink);

  // Check if pull sample failed
  if( sample==NULL )
  { 
    std::cout << "x\n";

    // Check state
    if( gst_app_sink_is_eos( appsink ) )
    { return GST_FLOW_ERROR; }
    
    // Unknown error
    return GST_FLOW_ERROR;
  }
      
  // Extract information from sample
  caps = gst_sample_get_caps(sample);
  buffer = gst_sample_get_buffer(sample);
  structure = gst_caps_get_structure(caps, 0);

  // Get frame data
  if(! gst_buffer_map(buffer, &map, GST_MAP_READ) )
  {
    std::cout << "xx\n";
    return GST_FLOW_CUSTOM_ERROR; 
  }

  width = g_value_get_int(gst_structure_get_value(structure, "width"));
  height = g_value_get_int(gst_structure_get_value(structure, "height"));

  // Convert gstreamer data to OpenCV Mat
  mu.lock();
  frame = cv::Mat( cv::Size(width, height), 
                   CV_8UC3, 
                   (char*)map.data, 
                   cv::Mat::AUTO_STEP ).clone();
  mu.unlock();

  // Clean up
  std::cout << ".\n";
  gst_buffer_unmap(buffer, &map);
  gst_sample_unref(sample);

  return GST_FLOW_OK;
}

bool SinkInterface::readFrame( cv::Mat& aframe)
{ 
  mu.lock();
  if( frame.empty() )
  { 
    mu.unlock();
    return false; 
  }

  aframe = frame.clone();
  mu.unlock();
  return true;
}
