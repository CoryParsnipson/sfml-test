#ifndef CLOSE_INPUT_EVENT_H
#define CLOSE_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// CloseInputEvent
//
// This event should be emitted when the window is being closed by the user.
// Common occurences where this event would be emitted are when the user clicks
// the 'x' in the upper right corner of the window or hits Ctrl+C
// ----------------------------------------------------------------------------
class CloseInputEvent : public InputEvent {
public:
   CloseInputEvent() : InputEvent("Close") {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const CloseInputEvent& event);
};

#endif
