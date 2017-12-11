#ifndef MOVE_LEFT_INTENT_H
#define MOVE_LEFT_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// MoveLeftIntent
// 
// This intent signals that the player is trying to move the entity attached
// to this Input component containing this intent to the left.
// ----------------------------------------------------------------------------
class MoveLeftIntent : public Intent {
public:
   MoveLeftIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("MoveLeftIntent", device_id, input_element)
   {
   }

   virtual ~MoveLeftIntent() {}
};

#endif
