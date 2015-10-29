#ifndef MOUSE_WHEEL_COMMAND_H
#define MOUSE_WHEEL_COMMAND_H

#include "../dependencies.h"
#include "Command.h"

class MouseWheelCommand : public Command {
public:
   MouseWheelCommand(sf::Event::MouseWheelEvent& e);

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   };

   // positive is up, negative is down
   int delta;

   // relative to left and top of owner window
   int x;
   int y;
};

#endif
