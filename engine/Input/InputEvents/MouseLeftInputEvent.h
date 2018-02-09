#ifndef MOUSE_LEFT_INPUT_EVENT_H
#define MOUSE_LEFT_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// MouseLeftInputEvent
//
// This event should be emitted when the window has focus and the user moves
// the mouse cursor from this window to a section of the screen that has other
// os windows.
// ----------------------------------------------------------------------------
class MouseLeftInputEvent : public InputEvent {
public:
   MouseLeftInputEvent() : InputEvent("MouseLeft") {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const MouseLeftInputEvent& event);
};

#endif
