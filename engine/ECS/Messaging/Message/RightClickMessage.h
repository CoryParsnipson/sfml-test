#ifndef RIGHT_CLICK_MESSAGE_H
#define RIGHT_CLICK_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// RightClickMessage
//
// This is a message that is sent when the right mouse button is clicked
// ----------------------------------------------------------------------------
class RightClickMessage : public Message {
public:
   RightClickMessage(float x, float y) : Message("RightClickMessage"), pos(x, y) {}
   RightClickMessage(sf::Vector2f pos) : Message("RightClickMessage"), pos(pos) {}

   sf::Vector2f pos;
};

#endif
