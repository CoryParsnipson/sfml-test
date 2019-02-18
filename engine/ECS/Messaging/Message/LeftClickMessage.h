#ifndef LEFT_CLICK_MESSAGE_H
#define LEFT_CLICK_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// LeftClickMessage
//
// This is a message that is sent when the left mouse button is clicked down
// ----------------------------------------------------------------------------
class LeftClickMessage : public Message {
public:
   LeftClickMessage(float x, float y) : Message("LeftClickMessage"), pos(x, y) {}
   LeftClickMessage(sf::Vector2f pos) : Message("LeftClickMessage"), pos(pos) {}

   sf::Vector2f pos;
};

#endif
