#include "InputListener.h"
#include "InputEvent.h"

InputListener::InputListener(std::string id /* = "InputListener" */)
: Observer<InputEvent>(id)
{
}

InputListener::~InputListener() {
}

void InputListener::notify(InputEvent& event) {
   event.execute(*this);
}
