#ifndef INTENT_H
#define INTENT_H

#include <memory>

#include "InputDevice.h"
#include "InputElement.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Intent;

// ----------------------------------------------------------------------------
// typedefs
// ----------------------------------------------------------------------------
typedef std::shared_ptr<Intent> IntentPtr;

// ----------------------------------------------------------------------------
// Intent
//
// This is an abstract form of input information. Instead of working with
// input elements from input devices (e.g. left click, A button, etc), you
// work with higher level user defined "intents". Examples of these things
// could be "MoveLeftIntent", or "JumpIntent" for instance. The intended
// usage of this class is to inherit from this class to create each new
// intent.
// ----------------------------------------------------------------------------
class Intent {
public:
   Intent(const std::string& id, InputDevice::DeviceId device_id, InputElementPtr input_element)
   : device_id_(device_id)
   , input_element_(input_element)
   {
   }

   virtual ~Intent() {}

   InputDevice::DeviceId id() const {
      return this->device_id_;
   }

   InputElementPtr element() const {
      return this->input_element_;
   }

private:
   InputDevice::DeviceId device_id_;
   InputElementPtr input_element_;
};

#endif
