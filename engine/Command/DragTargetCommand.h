#ifndef DRAG_TARGET_COMMAND_H
#define DRAG_TARGET_COMMAND_H

#include "Input.h"
#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class DragCommand;

// ----------------------------------------------------------------------------
// DragTargetCommand
//
// Given a DragCommand and a Moveable object, set the target of the drag command
// ----------------------------------------------------------------------------
class DragTargetCommand : public Command {
public:
   DragTargetCommand(DragCommand* command, Moveable* moveable = nullptr);
   virtual ~DragTargetCommand();

   virtual void execute();
   virtual void unexecute();
   
   void moveable(Moveable* moveable);

private:
   Moveable* moveable_;
   DragCommand* drag_command_;

   Moveable* old_moveable_;
};

#endif
