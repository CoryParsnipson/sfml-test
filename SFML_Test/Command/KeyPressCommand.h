#ifndef KEYPRESS_COMMAND_H
#define KEYPRESS_COMMAND_H

#include "../dependencies.h"
#include "Command.h"

class KeyPressCommand : public Command {
public:
   KeyPressCommand(sf::Event::KeyEvent& e);

   virtual void execute(InputListener& listener) {
      listener.process(*this);
   }

   sf::Event::KeyEvent& event;
};

#endif
