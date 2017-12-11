#ifndef MOVE_RIGHT_INTENT_H
#define MOVE_RIGHT_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// MoveRightIntent
// 
// This intent signals that the player is trying to move the entity attached
// to this Input component containing this intent to the left.
// ----------------------------------------------------------------------------
class MoveRightIntent : public Intent {
public:
   MoveRightIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("MoveRightIntent", device_id, input_element)
   {
   }

   virtual ~MoveRightIntent() {}
};

#endif

