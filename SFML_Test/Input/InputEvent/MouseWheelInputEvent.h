#ifndef MOUSE_WHEEL_INPUT_EVENT_H
#define MOUSE_WHEEL_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

class MouseWheelInputEvent : public InputEvent {
public:
   const int x;
   const int y;
   const int delta;

   MouseWheelInputEvent(int x, int y, int delta)
   : InputEvent("MouseWheel")
   , x(x)
   , y(y)
   , delta(delta)
   {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

private:
   friend std::ostream& operator<<(std::ostream& stream, const MouseWheelInputEvent& event);
};

#endif
