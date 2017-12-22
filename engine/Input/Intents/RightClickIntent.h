#ifndef RIGHT_CLICK_INTENT_H
#define RIGHT_CLICK_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// RightClickIntent
// 
// This intent signals that the player is trying to make a right mouse click
// or right tap.
// ----------------------------------------------------------------------------
class RightClickIntent : public Intent {
public:
   RightClickIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("RightClickIntent", device_id, input_element)
   {
   }

   virtual ~RightClickIntent() {}
};

#endif

