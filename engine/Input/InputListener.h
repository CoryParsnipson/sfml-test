#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "Observer.h"

class InputEvent;
class CloseInputEvent;
class ResizeInputEvent;
class KeyPressInputEvent;
class KeyReleaseInputEvent;
class MouseMoveInputEvent;
class MouseWheelInputEvent;
class MouseButtonInputEvent;

class InputListener
: public Observer<InputEvent>
{
public:
   InputListener(std::string id = "InputListener");
   virtual ~InputListener();

   // observer interface
   virtual void notify(InputEvent& event);

   // input event processing
   virtual void process(CloseInputEvent& e) = 0;
   virtual void process(ResizeInputEvent& e) = 0;
   virtual void process(KeyPressInputEvent& e) = 0;
   virtual void process(KeyReleaseInputEvent& e) = 0;
   virtual void process(MouseMoveInputEvent& e) = 0;
   virtual void process(MouseWheelInputEvent& e) = 0;
   virtual void process(MouseButtonInputEvent& e) = 0;
};

#endif
