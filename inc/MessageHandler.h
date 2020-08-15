#ifndef MESSAGEHANDLER
#define MESSAGEHANDLER

#include <iostream>
#include <vector>
#include <mutex>
#include <map>
#include <memory>
#include <gst/gst.h>
#include <gst/app/app.h>

namespace GStreamer
{

class MessageHandler
{
  public:
  //GstMessage* pState;

  void init();
  void setMap(const std::vector< int >&);
  bool setMessage(int, GstMessage*);
  bool getMessage(int, GstMessage*);
  static gboolean MessageCallback(GstBus*, GstMessage*, gpointer);

  private:
  std::mutex _mu;
  GstMessage* nullMessage;
  GstMessage* testMessage;
  std::map< int, std::unique_ptr< GstMessage >> messageMap;
  std::map< int, std::unique_ptr< GstMessage >>::iterator iMessage;

  std::map< int, GstMessage* > pMessageMap;
  std::map< int, GstMessage* >::iterator ipMessage;
};

}
#endif
