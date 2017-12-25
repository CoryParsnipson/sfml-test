#ifndef MOUSE_WHEEL_INTENT_H
#define MOUSE_WHEEL_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// MouseWheelIntent
// 
// This intent signals that the player is scrolling the mouse wheel.
// ----------------------------------------------------------------------------
class MouseWheelIntent : public Intent {
public:
   MouseWheelIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("MouseWheelIntent", device_id, input_element)
   {
   }

   virtual ~MouseWheelIntent() {}
};

#endif
