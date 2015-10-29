#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "../dependencies.h"

// forward declarations
class Command;
class CloseCommand;
class KeyPressCommand;
class WindowResizeCommand;
class MouseMoveCommand;
class MouseButtonCommand;
class MouseWheelCommand;

class InputListener; // needed for vector of pointers to other input listeners

class InputListener {
public:
   void registerInputListener(InputListener& listener);
   
   //--------------------------------------------------------------------------
   // command processing interface
   //--------------------------------------------------------------------------
   virtual void process(CloseCommand& c) = 0;
   virtual void process(KeyPressCommand& c) = 0;
   virtual void process(WindowResizeCommand& c) = 0;
   virtual void process(MouseMoveCommand& c) = 0;
   virtual void process(MouseButtonCommand& c) = 0;
   virtual void process(MouseWheelCommand& c) = 0;

protected:
   std::vector<InputListener*> listeners;
};

#endif
