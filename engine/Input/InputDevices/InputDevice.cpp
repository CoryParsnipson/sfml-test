#include "InputDevice.h"
#include "Game.h"

// ----------------------------------------------------------------------------
// static member initialization
// ----------------------------------------------------------------------------
InputDevice::DeviceId InputDevice::next_device_id = 0;

void InputDevice::update(Game& game) {
   for (ElementMap::const_iterator it = this->elements_.begin(); it != this->elements_.end(); ++it) {
      it->second->update(game);
   }
}
