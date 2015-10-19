#include "InputListener.h"

void InputListener::registerInputListener(InputListener& listener) {
   listeners.push_back(&listener);
}

void InputListener::push(Command& c) {
   // pass onto all subscribed listeners
   std::vector<InputListener*>::iterator it;
   for(it = this->listeners.begin(); it != this->listeners.end(); it++) {
      (*it)->push(c);
   }
}
