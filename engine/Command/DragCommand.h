#ifndef DRAG_COMMAND_H
#define DRAG_COMMAND_H

#include "Input.h"
#include "Command.h"
#include "MoveCommand.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;

// ----------------------------------------------------------------------------
// Drag command
//
// Move an specified objects based on Gamepad cursor movement
// ----------------------------------------------------------------------------
class DragCommand : public Command {
public:
   DragCommand(Gamepad* gamepad, Moveable* moveable = nullptr);
   virtual ~DragCommand();

   virtual void execute();
   virtual void unexecute();

   void moveable(Moveable* moveable);
   Moveable* moveable();

private:
   Gamepad* gamepad_;
   MoveCommand move_command_;
};

#endif
