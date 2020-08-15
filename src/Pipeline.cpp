#include "Pipeline.h"

GStreamerPipeline::GStreamerPipeline()
{
  pError = NULL;

  if(! gst_init_check(NULL, NULL, &pError ) )
  { throw 1001; }

  //pState = gst_message_new_state_changed(NULL, GST_STATE_NULL, GST_STATE_NULL, GST_STATE_NULL); 
  pSink = std::make_shared< SinkInterface >(); 
  pBus = std::make_shared< GStreamerBus >();
}

int GStreamerPipeline::getState()
{
  pBus->getState( _state );
  //GstState state = gst_element_get_state(pPipeline); 
  //GstState state = GST_STATE(pPipeline); 
  std::cout << _state.current << "\n";
  return _state.current;
}

void GStreamerPipeline::createFromDescription(const std::string& pipeDesc)
{
  pPipeline = gst_parse_launch(pipeDesc.c_str(), &pError ); 

  // Throw if fail
  if(pPipeline == NULL)
  { throw 1002; }

  // Setup sink element from pipeline
  pSink->initFromPipe( pPipeline );

  // Attach message bus to pipeline
  pBus->init( pPipeline );
}

bool GStreamerPipeline::readFrame( cv::Mat& frame )
{ 
  // Read frame from sink
  if(! pSink->readFrame( frame ) ) 
  { return false; }

  return true;
}

void GStreamerPipeline::start()
{ gst_element_set_state( GST_ELEMENT(pPipeline), GST_STATE_PLAYING); }

void GStreamerPipeline::stop()
{ 
  pBus->dereference();
  gst_element_set_state( GST_ELEMENT(pPipeline), GST_STATE_NULL );
  pSink->deref();
  gst_object_unref( GST_OBJECT(pPipeline) );
  pPipeline = NULL;
}
