#include "MessageHandler.h"

namespace GStreamer
{

void MessageHandler::init()
{ nullMessage = NULL; }

void MessageHandler::setMap(const std::vector<int>& vKey)
{
  std::vector<int>::const_iterator iKey = vKey.begin();
/*
  for(; iKey!=vKey.end(); iKey++)
  {
    messageMap.emplace( std::make_pair(*iKey, std::make_unique<GstMessage>() ));
  }
*/
  for(; iKey!=vKey.end(); iKey++)
  {
    pMessageMap.emplace( std::make_pair(*iKey, nullMessage) );
  }
}

bool MessageHandler::setMessage(int key, GstMessage* pMessage)
{
  //std::lock_guard<std::mutex> guard(_mu);
  GST_MESSAGE_LOCK(pMessage);

  std::cout << "Enter\n";
  ipMessage = pMessageMap.find(key);

  if(ipMessage == pMessageMap.end())
  { return false; }

/*
  GstMessage* oldMessage = iMessage->second.release();

  if( GST_MESSAGE_TYPE( oldMessage ) != 0 )
  { gst_message_unref( oldMessage ); }
*/

  //gst_message_replace( &(ipMessage->second), pMessage );
  ipMessage->second = gst_message_copy(pMessage);
  GST_MESSAGE_UNLOCK( pMessage );

  //iMessage->second.reset( gst_message_copy(pMessage) );

  /*
  GstMessage** ppMessage = &(iMessage->second.get());
  gst_message_replace( ppMessage, pMessage);
  iMessage->second.reset( *ppMessage );
  */
/*
  if( GST_MESSAGE_TYPE( ipMessage->second ) == GST_MESSAGE_STATE_CHANGED )
  {
    GstState* previous;
    GstState* current;
    gst_message_parse_state_changed(ipMessage->second, previous, current, NULL );
    std::cout << "state: " << *current << "\n";
    testMessage = gst_message_ref( ipMessage->second );
  }
*/

  if( GST_MESSAGE_TYPE( pMessage ) == GST_MESSAGE_ERROR )
  {
    GError *err = NULL;
    gchar *dbg_info = NULL;

    gst_message_parse_error (pMessage, &err, &dbg_info);
    g_printerr ("ERROR from element %s: %s\n",
    GST_OBJECT_NAME (pMessage->src), err->message);
    g_printerr ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
    g_error_free (err);
    g_free (dbg_info);
  }

  return true;
}

bool MessageHandler::getMessage(int key, GstMessage* pMessage)
{
  std::lock_guard<std::mutex> guard(_mu);

  ipMessage = pMessageMap.find(key);

  if(ipMessage == pMessageMap.end() || ipMessage->second == NULL)
  { return false; }

  // If haven't received type of message requested type=0
  if( GST_MESSAGE_TYPE( ipMessage->second ) != key )
  { return false; }
  
  //pMessage = gst_message_ref(iMessage->second.get());

  // If new and old message are same, return false
  if(! gst_message_replace( &(pMessage), ipMessage->second ) )
  { return false; }

  if( GST_MESSAGE_TYPE( ipMessage->second ) == GST_MESSAGE_STATE_CHANGED )
  {
    GstState* previous;
    GstState* current;
    gst_message_parse_state_changed(ipMessage->second, previous, current, NULL );
    std::cout << "state: " << *current << "\n";
  }

  std::cout << "Replace\n";
  
  return true;
}

gboolean MessageHandler::MessageCallback(GstBus* pBus, GstMessage* pMessage, gpointer pMessageHandler)
{ 
  // Recast as true type
  MessageHandler* pMH = (MessageHandler*)pMessageHandler;

  // Copy message to struct with timestamp
  if(! pMH->setMessage( GST_MESSAGE_TYPE(pMessage), pMessage) )
  { 
    std::cout << "Message does not have key value" << GST_MESSAGE_TYPE(pMessage) << "\n";
    //spdlog::get("Messages")->info("Message ID {} does not have key value", msg.msgid); 
  }

  //gst_message_unref(pMessage);
  std::cout << "Exit\n";
  // Decode and log message
  //if( pLogger != NULL)
  //{ pLogger->logMessage(msg); }

  return true;
}

}
