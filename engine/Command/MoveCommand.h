#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <string>

#include "sfml.h"

#include "InputManager.h"
#include "Command.h"

// ----------------------------------------------------------------------------
// move command
// ----------------------------------------------------------------------------
class MoveCommand : public Command {
public:
   MoveCommand(Moveable* moveable, sf::Vector2f delta = sf::Vector2f(0, 0), std::string id = "MoveCommand");
   virtual ~MoveCommand();
   
   virtual void execute();
   virtual void unexecute();

   void delta(sf::Vector2f delta);
   const sf::Vector2f& delta();

   void moveable(Moveable* moveable);
   Moveable* moveable();

private:
   sf::Vector2f delta_;
   Moveable* moveable_;
};

#endif
