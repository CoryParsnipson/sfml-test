#ifndef RIGHT_MOUSE_CLICK_COMMAND_H
#define RIGHT_MOUSE_CLICK_COMMAND_H

#include "Command.h"
#include "GetSceneObjectCommand.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;
class SceneObject;

// ----------------------------------------------------------------------------
// RightMouseClickCommand
//
// This command represents default right mouse click behavior of gamepads. It
// will search through the scene graph for any objects intersecting with the
// gamepad cursor and then run the on_right_click() callback of the topmost
// intersecting SceneObject.
// ----------------------------------------------------------------------------
class RightMouseClickCommand : public Command {
public:
   RightMouseClickCommand(Gamepad* g, SceneObject* scene_graph);
   virtual ~RightMouseClickCommand();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   Gamepad* gamepad_;
   GetSceneObjectCommand target_;
};

#endif
