#ifndef LEFT_RELEASE_MESSAGE_H
#define LEFT_RELEASE_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// LeftReleaseMessage
//
// This is a message that is sent when the left mouse button is released
// ----------------------------------------------------------------------------
class LeftReleaseMessage : public Message {
public:
   LeftReleaseMessage(float x, float y, float release_x, float release_y)
   : Message("LeftReleaseMessage")
   , click_pos(x, y)
   , release_pos(release_x, release_y)
   {}

   LeftReleaseMessage(sf::Vector2f click_pos, sf::Vector2f release_pos)
   : Message("LeftReleaseMessage")
   , click_pos(click_pos)
   , release_pos(release_pos)
   {}

   sf::Vector2f click_pos;
   sf::Vector2f release_pos;
};

#endif
