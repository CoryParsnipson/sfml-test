#ifndef RESIZE_CAMERA_MESSAGE
#define RESIZE_CAMERA_MESSAGE

#include <SFML/System/Vector2.hpp>

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// ResizeCameraMessage
//
// This is message that tells any subscribed systems to resize their camera
// to the specified size.
// ----------------------------------------------------------------------------
class ResizeCameraMessage : public Message {
public:
   ResizeCameraMessage()
   : Message("ResizeCameraMessage")
   , width(0)
   , height(0)
   {}

   ResizeCameraMessage(int width, int height)
   : Message("ResizeCameraMessage")
   , width(width)
   , height(height)
   {}

   ResizeCameraMessage(sf::Vector2f size)
   : Message("ResizeCameraMessage")
   , width(size.x)
   , height(size.y)
   {}

   int width;
   int height;
};

#endif
