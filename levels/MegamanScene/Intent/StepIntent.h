#ifndef STEP_INTENT_H
#define STEP_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// StepIntent
//
// This intent signals that the player wants to advance the game one
// game loop iteration.
// ----------------------------------------------------------------------------
class StepIntent : public Intent {
public:
   StepIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("StepIntent", device_id, input_element)
   {
   }

   virtual ~StepIntent() {}
};

#endif
