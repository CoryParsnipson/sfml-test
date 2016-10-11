#ifndef MOUSE_WHEEL_COMMAND_H
#define MOUSE_WHEEL_COMMAND_H 

#include "Command.h"
#include "GetSceneObjectCommand.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;
class SceneObject;

// ----------------------------------------------------------------------------
// MouseWheelCommand 
//
// This command represents default mouse wheel behavior of gamepads. It
// will search through the scene graph for any objects intersecting with the
// gamepad cursor and then run the on_mouse_wheel() callback of the topmost
// intersecting SceneObject.
// ----------------------------------------------------------------------------
class MouseWheelCommand : public Command {
public:
   MouseWheelCommand(Gamepad* g, SceneObject* scene_graph);
   virtual ~MouseWheelCommand();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   Gamepad* gamepad_;
   GetSceneObjectCommand target_;
};

#endif

