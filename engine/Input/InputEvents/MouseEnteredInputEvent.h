#ifndef MOUSE_ENTERED_INPUT_EVENT_H
#define MOUSE_ENTERED_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// MouseEnteredInputEvent
//
// This event should be emitted when the window has focus and the user moves
// the mouse cursor from a section of the screen that contains other os
// windows onto this window.
// ----------------------------------------------------------------------------
class MouseEnteredInputEvent : public InputEvent {
public:
   MouseEnteredInputEvent() : InputEvent("MouseEntered") {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const MouseEnteredInputEvent& event);
};

#endif
