#ifndef SET_VISIBLE_COMMAND_H
#define SET_VISIBLE_COMMAND_H

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class SceneObject;

// ----------------------------------------------------------------------------
// SetVisibleCommand
//
// Set the visibility of a scene object
// ----------------------------------------------------------------------------
class SetVisibleCommand : public Command {
public:
   SetVisibleCommand(SceneObject* scene_object, bool set_visible);
   virtual ~SetVisibleCommand();

   virtual void execute();
   virtual void unexecute();

private:
   bool new_visible_state_;
   bool old_visible_state_;
   SceneObject* scene_object_;
};

#endif
