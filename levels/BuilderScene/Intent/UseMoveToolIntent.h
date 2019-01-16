#ifndef USE_MOVE_TOOL_INTENT
#define USE_MOVE_TOOL_INTENT

#include "Intent.h"

// ----------------------------------------------------------------------------
// UseMoveToolIntent
//
// This intent signals that the player is pressing down the key to switch
// the mouse behavior to the move tool.
// ----------------------------------------------------------------------------
class UseMoveToolIntent : public Intent {
public:
   UseMoveToolIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("UseMoveToolIntent", device_id, input_element)
   {
   }

   virtual ~UseMoveToolIntent() {}
};

#endif
