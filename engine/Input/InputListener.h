#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "Observer.h"
#include "InputEvent.h"

class CloseInputEvent;
class LostFocusInputEvent;
class GainedFocusInputEvent;
class MouseEnteredInputEvent;
class MouseLeftInputEvent;
class ResizeInputEvent;
class KeyPressInputEvent;
class KeyReleaseInputEvent;
class MouseMoveInputEvent;
class MouseWheelInputEvent;
class MouseButtonInputEvent;

class InputListener
: public Observer<InputEventPtr>
{
public:
   InputListener(std::string id = "InputListener") : Observer<InputEventPtr>(id) {}
   virtual ~InputListener() {}

   // observer interface
   virtual void notify(InputEventPtr& event) {
      event->execute(*this);
   }

   // input event processing
   virtual void process(CloseInputEvent& e) = 0;
   virtual void process(LostFocusInputEvent& e) = 0;
   virtual void process(GainedFocusInputEvent& e) = 0;
   virtual void process(MouseEnteredInputEvent& e) = 0;
   virtual void process(MouseLeftInputEvent& e) = 0;
   virtual void process(ResizeInputEvent& e) = 0;
   virtual void process(KeyPressInputEvent& e) = 0;
   virtual void process(KeyReleaseInputEvent& e) = 0;
   virtual void process(MouseMoveInputEvent& e) = 0;
   virtual void process(MouseWheelInputEvent& e) = 0;
   virtual void process(MouseButtonInputEvent& e) = 0;
};

#endif
