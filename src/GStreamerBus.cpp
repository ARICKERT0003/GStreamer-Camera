#include "GStreamerBus.h"

/**
* @brief Bus callback
*  Print important messages
*
* @param bus
* @param message
* @param data
* @return gboolean
*/
/*
static gboolean my_bus_callback(GstBus* pBus, GstMessage* pMessage, gpointer pState  )
{
  // Recast gpointer to GstMessage*
  GstMessage*(pState)

  // Debug message
  //g_print("Got %s message\n", GST_MESSAGE_TYPE_NAME(message));
  switch( GST_MESSAGE_TYPE(pMessage) ) 
  {

    case GST_MESSAGE_ERROR: 
    {
      GError *err;
      gchar *debug;
      gst_message_parse_error(pMessage, &err, &debug);
      g_print("Error: %s\n", err->message);
      g_error_free(err);
      g_free(debug);
      break;
    }

    case GST_MESSAGE_EOS:
    {
      // end-of-stream 
      break;
    }

    case GST_MESSAGE_STATE_CHANGED:
    { 
      *pState = *pMessage
      //gst_message_parse_state_changed(pMessage, &oldState, &newState, NULL); 
      break;
    }

    default:
    {
      // unhandled message 
      std::cout << GST_MESSAGE_TYPE(pMessage) << "\n";
      break;
    }

  }
  // we want to be notified again the next time there is a message
  //on the bus, so returning TRUE (FALSE means we want to stop watching
  //for messages on the bus and our callback should not be called again)
  //
  return true;
}
*/

void GStreamerBus::init( GstElement* pipeline )
{
  pBus = gst_pipeline_get_bus( GST_PIPELINE(pipeline) );
  pMessageHandler = std::make_shared< GStreamer::MessageHandler >();
  pMessageHandler->setMap( GStreamer::vMessageType );
  gst_bus_add_watch( pBus, pMessageHandler->MessageCallback, pMessageHandler.get() );
  pMessage = NULL;
}

/*
void GStreamerBus::attachCallbacks( GstElement* pipeline, GstMessage* pState )
{
  pBus = gst_pipeline_get_bus( GST_PIPELINE(pipeline) );
  gst_bus_add_watch(pBus, pMessageHandler->MessageCallback, pMessageHandler);
}
*/

void GStreamerBus::dereference()
{ 
  gst_object_unref( pBus ); 
  pBus = NULL;
}

// ====================================
// Getters
// ====================================
void GStreamerBus::getState( GStreamer::State& state )
{
  //pMessage->type = GST_MESSAGE_STATE_CHANGED;
  if(! pMessageHandler->getMessage(GST_MESSAGE_STATE_CHANGED, pMessage) )
  { return; }

  gst_message_parse_state_changed(pMessage, &(state.previous), &(state.current), &(state.pending) );
}
