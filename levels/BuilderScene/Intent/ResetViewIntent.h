#ifndef RESET_VIEW_INTENT_H
#define RESET_VIEW_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// ResetViewIntent
// 
// This intent signals that the player is trying to change the visibility of
// a grid component.
// ----------------------------------------------------------------------------
class ResetViewIntent : public Intent {
public:
   ResetViewIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("ResetViewIntent", device_id, input_element)
   {
   }

   virtual ~ResetViewIntent() {}
};

#endif
