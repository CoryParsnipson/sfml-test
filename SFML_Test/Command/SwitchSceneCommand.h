#ifndef SWITCH_SCENE_COMMAND_H
#define SWITCH_SCENE_COMMAND_H

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Scene;

// ----------------------------------------------------------------------------
// SwitchSceneCommand
//
// Unload the current scene and load a specified scene. This will have no
// effect if the scene to load is a nullptr.
// ----------------------------------------------------------------------------
class SwitchSceneCommand : public Command {
public:
   SwitchSceneCommand(Scene* scene = nullptr);
   virtual ~SwitchSceneCommand();

   virtual void execute();
   virtual void unexecute();

private:
   Scene* next_scene_;
};

#endif
