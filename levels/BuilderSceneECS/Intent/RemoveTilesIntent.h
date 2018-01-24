#ifndef REMOVE_TILES_INTENT_H
#define REMOVE_TILES_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// RemoveTilesIntent
// 
// This intent signals that the player is trying to remove tiles from a
// specified tilemap.
// ----------------------------------------------------------------------------
class RemoveTilesIntent : public Intent {
public:
   RemoveTilesIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("RemoveTilesIntent", device_id, input_element)
   {
   }

   virtual ~RemoveTilesIntent() {}
};

#endif
