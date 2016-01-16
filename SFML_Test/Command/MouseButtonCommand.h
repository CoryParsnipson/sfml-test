#ifndef MOUSE_BUTTON_COMMAND_H
#define MOUSE_BUTTON_COMMAND_H

#include "dependencies.h"

#include "Command.h"

class MouseButtonCommand : public Command {
public:
   enum STATE {
      PRESSED,
      RELEASED
   };

   enum MOUSE_BUTTON {
      LEFT,
      RIGHT,
      MIDDLE,
      XBUTTON1,
      XBUTTON2 
   }; 

   MouseButtonCommand(sf::Event::MouseButtonEvent& e, MouseButtonCommand::STATE state);
   
   virtual void execute(InputListener& listener) {
      listener.process(*this);
   };

   STATE state;
   MOUSE_BUTTON button;

   // positions are relative to top and left of owner window
   int x;
   int y;

   std::string to_string();
};

#endif
