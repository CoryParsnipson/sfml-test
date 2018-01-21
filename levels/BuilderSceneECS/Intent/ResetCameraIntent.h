#ifndef RESET_CAMERA_INTENT_H
#define RESET_CAMERA_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// ResetCameraIntent
// 
// This intent signals that the player is trying to change the visibility of
// a grid component.
// ----------------------------------------------------------------------------
class ResetCameraIntent : public Intent {
public:
   ResetCameraIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("ResetCameraIntent", device_id, input_element)
   {
   }

   virtual ~ResetCameraIntent() {}
};

#endif
