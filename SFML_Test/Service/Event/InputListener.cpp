#include "InputListener.h"

void InputListener::registerInputListener(InputListener& listener) {
   listeners_.push_back(&listener);
}

void InputListener::unregisterInputListener(InputListener& listener) {
   InputListener::InputListenerList::iterator it;
   for (it = this->listeners_.begin(); it != this->listeners_.end(); ++it) {
      if (*it != &listener) {
         continue;
      }

      this->listeners_.erase(it);
      return;
   }
}
