#ifndef MOUSE_MOVE_COMMAND_H
#define MOUSE_MOVE_COMMAND_H

#include "../dependencies.h"
#include "Command.h"

class MouseMoveCommand : public Command {
public:
   MouseMoveCommand(sf::Event::MouseMoveEvent& e);

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   };

   // from mouse move event, relative to left and top of owner window
   int x;
   int y; 
};

#endif
