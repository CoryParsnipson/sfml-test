#ifndef USE_CLONE_TOOL_INTENT
#define USE_CLONE_TOOL_INTENT

#include "Intent.h"

// ----------------------------------------------------------------------------
// UseCloneToolIntent
//
// This intent signals that the player is pressing down the key to switch
// the mouse behavior to the clone tool.
// ----------------------------------------------------------------------------
class UseCloneToolIntent : public Intent {
public:
   UseCloneToolIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("UseCloneToolIntent", device_id, input_element)
   {
   }

   virtual ~UseCloneToolIntent() {}
};

#endif
