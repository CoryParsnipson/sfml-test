#ifndef WINDOW_RESIZE_COMMAND_H
#define WINDOW_RESIZE_COMMAND_H

#include "../dependencies.h"
#include "Command.h"

class WindowResizeCommand : public Command {
public:
   WindowResizeCommand(sf::Event::SizeEvent& e);
   
   virtual void execute(InputListener& listener) {
      listener.process(*this);
   };

   int width;
   int height;
};

#endif
