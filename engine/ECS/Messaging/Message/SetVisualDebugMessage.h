#ifndef SET_VISUAL_DEBUG_MESSAGE_H
#define SET_VISUAL_DEBUG_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// SetVisualDebugMessage
//
// This message lets you enable or disable the visual debug system.
// ----------------------------------------------------------------------------
class SetVisualDebugMessage : public Message {
public:
   SetVisualDebugMessage(bool enable) : enable(enable) {}

   bool enable;
};

#endif

