#ifndef MOUSE_MOVE_COMMAND_H
#define MOUSE_MOVE_COMMAND_H

#include "Command.h"
#include "GetSceneObjectCommand.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;
class SceneObject;

// ----------------------------------------------------------------------------
// MouseMoveCommand
//
// This command represents default mouse release behavior of gamepads. It
// will search through the scene graph for any objects intersecting with the
// gamepad cursor and then run the on_mouse_move() callback of the topmost
// intersecting SceneObject.
// ----------------------------------------------------------------------------
class MouseMoveCommand : public Command {
public:
   MouseMoveCommand(Gamepad* g, SceneObject* scene_graph);
   virtual ~MouseMoveCommand();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   Gamepad* gamepad_;
   GetSceneObjectCommand target_;
};

#endif

