#ifndef LEFT_CLICK_INTENT_H
#define LEFT_CLICK_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// LeftClickIntent
// 
// This intent signals that the player is trying to make a left mouse click
// or left tap.
// ----------------------------------------------------------------------------
class LeftClickIntent : public Intent {
public:
   LeftClickIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("LeftClickIntent", device_id, input_element)
   {
   }

   virtual ~LeftClickIntent() {}
};

#endif
