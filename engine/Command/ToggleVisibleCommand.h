#ifndef TOGGLE_VISIBLE_COMMAND_H
#define TOGGLE_VISIBLE_COMMAND_H

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class SceneObject;

// ----------------------------------------------------------------------------
// ToggleVisibleCommand
// 
// reverse the visibility of a scene object
// ----------------------------------------------------------------------------
class ToggleVisibleCommand : public Command {
public:
   ToggleVisibleCommand(SceneObject* scene_object);
   virtual ~ToggleVisibleCommand();

   virtual void execute();
   virtual void unexecute();

private:
   SceneObject* scene_object_;
};

#endif
