#ifndef MOUSE_MOVE_INPUT_EVENT_H
#define MOUSE_MOVE_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// MouseMoveInputEvent
//
// This event should be emitted when the mouse cursor moves over the game
// window. The event contains x and y coordinates of the mouse cursor's latest 
// position relative to the origin of the game window coordinate system.
// ----------------------------------------------------------------------------
class MouseMoveInputEvent : public InputEvent {
public:
   const int x;
   const int y;

   MouseMoveInputEvent(int x, int y)
   : InputEvent("MouseMove")
   , x(x)
   , y(y)
   {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const MouseMoveInputEvent& event);
};

#endif
