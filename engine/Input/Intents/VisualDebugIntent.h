#ifndef VISUAL_DEBUG_INTENT_H
#define VISUAL_DEBUG_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// VisualDebugIntent
// 
// This intent signals that the player is trying to enable or disable the
// visual debug system.
// ----------------------------------------------------------------------------
class VisualDebugIntent : public Intent {
public:
   VisualDebugIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("VisualDebugIntent", device_id, input_element)
   {
   }

   virtual ~VisualDebugIntent() {}
};

#endif
