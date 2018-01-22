#ifndef MOVE_UP_INTENT_H
#define MOVE_UP_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// MoveUpIntent
// 
// This intent signals that the player is trying to move the entity attached
// to this Input component containing this intent upwards.
// ----------------------------------------------------------------------------
class MoveUpIntent : public Intent {
public:
   MoveUpIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("MoveUpIntent", device_id, input_element)
   {
   }

   virtual ~MoveUpIntent() {}
};

#endif
