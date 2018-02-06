#ifndef SERIALIZE_MAP_INTENT_H
#define SERIALIZE_MAP_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// SerializeMapIntent
// 
// This intent signals that the player is trying to serialize a tilemap
// entity to a file.
// ----------------------------------------------------------------------------
class SerializeMapIntent : public Intent {
public:
   SerializeMapIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("SerializeMapIntent", device_id, input_element)
   {
   }

   virtual ~SerializeMapIntent() {}
};

#endif
