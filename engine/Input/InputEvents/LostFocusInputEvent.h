#ifndef LOST_FOCUS_INPUT_EVENT_H
#define LOST_FOCUS_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// LostFocusInputEvent
//
// This event should be emitted when the user clicks away from this window and
// onto a different window of the operating system.
// ----------------------------------------------------------------------------
class LostFocusInputEvent : public InputEvent {
public:
   LostFocusInputEvent() : InputEvent("LostFocus") {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const LostFocusInputEvent& event);
};

#endif
