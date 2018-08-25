#ifndef JUMP_INTENT_H
#define JUMP_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// JumpIntent
//
// This intent signals that the player wants to jump.
// ----------------------------------------------------------------------------
class JumpIntent : public Intent {
public:
   JumpIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("JumpIntent", device_id, input_element)
   {
   }

   virtual ~JumpIntent() {}
};

#endif
