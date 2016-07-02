#ifndef GET_WIDGET_COMMAND_H
#define GET_WIDGET_COMMAND_H

#include <map>
#include <vector>

#include "dependencies.h"
#include "Command.h"
#include "SceneGraphVisitor.h"
#include "Scene.h"
#include "Widget.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class SceneGraphNode;
class Gamepad;

// ----------------------------------------------------------------------------
// aliases
// ----------------------------------------------------------------------------
using WidgetList = std::vector<Widget*>;

// ----------------------------------------------------------------------------
// GetWidgetCommand class
//
// This command takes a reference to a scene graph and a coordinate. Upon
// execution, it will traverse the scene graph node and create a list of
// widgets in the scene graph that contain that coordinate. This command
// will not collect widgets that are not visible from the scene graph.
// ----------------------------------------------------------------------------
class GetWidgetCommand
: public Command
, public SceneGraphVisitor
{
public:
   GetWidgetCommand(Scene::SceneGraph* scene_graph, Gamepad* gamepad);
   virtual ~GetWidgetCommand();

   void scene_graph(Scene::SceneGraph* scene_graph);
   Scene::SceneGraph* scene_graph();

   void gamepad(Gamepad* gamepad);
   Gamepad* gamepad();

   const WidgetList& get();

   // command interface
   virtual void execute();
   virtual void unexecute();

   // scene graph visitor interface
   virtual void visit(Camera*);
   virtual void visit(Draw*);
   virtual void visit(Entity*);
   virtual void visit(Widget*);

protected:
   Scene::SceneGraph* scene_graph_;
   Gamepad* gamepad_; // TODO: UGH

   WidgetList widgets_;   
};

#endif
