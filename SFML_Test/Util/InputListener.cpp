#include "InputListener.h"

void InputListener::registerInputListener(InputListener& listener) {
   listeners.push_back(&listener);
}
