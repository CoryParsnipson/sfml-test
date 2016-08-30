#ifndef RESIZE_CAMERA_COMMAND_H
#define RESIZE_CAMERA_COMMAND_H

#include "Command.h"
#include "SceneGraphVisitor.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class SceneObject;

// ----------------------------------------------------------------------------
// ResizeCameraCommand
//
// Traverse a given scene graph and trigger a resize event on all its cameras.
// ----------------------------------------------------------------------------
class ResizeCameraCommand
: public Command
, public SceneGraphVisitor
{
public:
   ResizeCameraCommand(SceneObject* scene_graph);
   virtual ~ResizeCameraCommand();

   // command interface
   virtual void execute();
   virtual void unexecute();

   // scene graph visitor interface
   virtual void visit(Camera*);
   virtual void visit(Entity*);
   virtual void visit(Map*);
   virtual void visit(Grid*);
   virtual void visit(Widget*);
   virtual void visit(Gamepad*);

private:
   SceneObject* scene_graph_;
};

#endif
