#ifndef MOUSE_X_INTENT_H
#define MOUSE_X_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// MouseXIntent
// 
// This intent signals that the player is trying to move a mouse cursor type
// entity horizontally.
// ----------------------------------------------------------------------------
class MouseXIntent : public Intent {
public:
   MouseXIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("MouseXIntent", device_id, input_element)
   {
   }

   virtual ~MouseXIntent() {}
};

#endif
