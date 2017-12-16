#ifndef MOUSE_Y_INTENT_H
#define MOUSE_Y_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// MouseYIntent
// 
// This intent signals that the player is trying to move a mouse cursor type
// entity vertically.
// ----------------------------------------------------------------------------
class MouseYIntent : public Intent {
public:
   MouseYIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("MouseYIntent", device_id, input_element)
   {
   }

   virtual ~MouseYIntent() {}
};

#endif
