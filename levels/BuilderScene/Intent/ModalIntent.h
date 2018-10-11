#ifndef MODAL_INTENT_H
#define MODAL_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// ModalIntent
// 
// This intent signals that the player is pressing down the assigned "meta"
// key to perform system actions.
// ----------------------------------------------------------------------------
class ModalIntent : public Intent {
public:
   ModalIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("ModalIntent", device_id, input_element)
   {
   }

   virtual ~ModalIntent() {}
};

#endif
