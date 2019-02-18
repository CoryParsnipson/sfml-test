#ifndef RIGHT_RELEASE_MESSAGE_H
#define RIGHT_RELEASE_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// RightReleaseMessage
//
// This is a message that is sent when the right mouse button is released
// ----------------------------------------------------------------------------
class RightReleaseMessage : public Message {
public:
   RightReleaseMessage(float x, float y, float release_x, float release_y)
   : Message("RightReleaseMessage")
   , click_pos(x, y)
   , release_pos(release_x, release_y)
   {}

   RightReleaseMessage(sf::Vector2f click_pos, sf::Vector2f release_pos)
   : Message("RightReleaseMessage")
   , click_pos(click_pos)
   , release_pos(release_pos)
   {}

   sf::Vector2f click_pos;
   sf::Vector2f release_pos;
};

#endif
