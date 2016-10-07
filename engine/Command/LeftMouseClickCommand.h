#ifndef LEFT_MOUSE_CLICK_COMMAND_H
#define LEFT_MOUSE_CLICK_COMMAND_H

#include "Command.h"
#include "GetSceneObjectCommand.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;
class SceneObject;

// ----------------------------------------------------------------------------
// LeftMouseClickCommand
//
// This command represents default left mouse click behavior of gamepads. It
// will search through the scene graph for any objects intersecting with the
// gamepad cursor and then run the on_left_click() callback of the topmost
// intersecting SceneObject.
// ----------------------------------------------------------------------------
class LeftMouseClickCommand : public Command {
public:
   LeftMouseClickCommand(Gamepad* g, SceneObject* scene_graph);
   virtual ~LeftMouseClickCommand();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   Gamepad* gamepad_;
   GetSceneObjectCommand target_;
};

#endif
