#ifndef LEFT_MOUSE_RELEASE_COMMAND_H
#define LEFT_MOUSE_RELEASE_COMMAND_H

#include "Command.h"
#include "GetSceneObjectCommand.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;
class SceneObject;

// ----------------------------------------------------------------------------
// LeftMouseReleaseCommand
//
// This command represents default left mouse button release behavior of
// gamepads. It will search through the scene graph for any objects
// intersecting with the gamepad cursor and then run the on_left_release()
// callback of the topmost intersecting SceneObject.
// ----------------------------------------------------------------------------
class LeftMouseReleaseCommand : public Command {
public:
   LeftMouseReleaseCommand(Gamepad* g, SceneObject* scene_graph);
   virtual ~LeftMouseReleaseCommand();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   Gamepad* gamepad_;
   GetSceneObjectCommand target_;
};

#endif
