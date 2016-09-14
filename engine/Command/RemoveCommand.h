#ifndef REMOVE_COMMAND_H
#define REMOVE_COMMAND_H

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class SceneObject;

// ----------------------------------------------------------------------------
// RemoveCommand
//
// Given a scene graph and a scene object, remove the scene object from the
// scene graph. If the scene object is not in the scene graph, the scene
// graph is unchanged. All children of the scene object in question are also
// removed from the scene graph, BUT NOT FROM THE SCENE OBJECT.
// ----------------------------------------------------------------------------
class RemoveCommand : public Command {
public:
   RemoveCommand(SceneObject* scene_graph_, SceneObject* scene_object_);
   virtual ~RemoveCommand();
   
   virtual void execute();
   virtual void unexecute();

   SceneObject* scene_object();
   SceneObject* parent();

private:
   SceneObject* scene_object_;
   SceneObject* scene_graph_;

   int child_idx_;
   SceneObject* parent_node_;
};

#endif
