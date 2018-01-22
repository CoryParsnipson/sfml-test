#ifndef MOVE_DOWN_INTENT_H
#define MOVE_DOWN_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// MoveDownIntent
// 
// This intent signals that the player is trying to move the entity attached
// to this Input component containing this intent downwards.
// ----------------------------------------------------------------------------
class MoveDownIntent : public Intent {
public:
   MoveDownIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("MoveDownIntent", device_id, input_element)
   {
   }

   virtual ~MoveDownIntent() {}
};

#endif
