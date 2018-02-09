#ifndef GAINED_FOCUS_INPUT_EVENT_H
#define GAINED_FOCUS_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// GainedFocusInputEvent
//
// This event should be emitted when the window does not have focus and then
// the user clicks into it from another window of the operating system.
// ----------------------------------------------------------------------------
class GainedFocusInputEvent : public InputEvent {
public:
   GainedFocusInputEvent() : InputEvent("Close") {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const GainedFocusInputEvent& event);
};

#endif

