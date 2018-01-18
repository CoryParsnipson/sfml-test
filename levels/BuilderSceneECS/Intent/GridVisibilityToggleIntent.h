#ifndef GRID_VISIBILITY_TOGGLE_INTENT_H
#define GRID_VISIBILITY_TOGGLE_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// GridVisibilityToggleIntent
// 
// This intent signals that the player is trying to change the visibility of
// a grid component.
// ----------------------------------------------------------------------------
class GridVisibilityToggleIntent : public Intent {
public:
   GridVisibilityToggleIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("GridVisibilityToggleIntent", device_id, input_element)
   {
   }

   virtual ~GridVisibilityToggleIntent() {}
};

#endif

