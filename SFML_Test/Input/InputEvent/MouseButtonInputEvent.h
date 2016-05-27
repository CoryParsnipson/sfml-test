#ifndef MOUSE_BUTTON_INPUT_EVENT_H
#define MOUSE_BUTTON_INPUT_EVENT_H

#include "InputEvent.h"
#include "InputListener.h"

enum class MouseButton {
   Left,
   Right,
   Middle,
   XButton1,
   XButton2
};

enum class MouseButtonState {
   Pressed,
   Released
};

class MouseButtonInputEvent : public InputEvent {
public:
   const MouseButton button;
   const MouseButtonState state;
   const int x;
   const int y;

   MouseButtonInputEvent(MouseButton button, MouseButtonState state, int x, int y)
   : button(button)
   , state(state)
   , x(x)
   , y(y)
   {}

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }
};

#endif
