#ifndef GET_WIDGET_COMMAND_H
#define GET_WIDGET_COMMAND_H

#include <map>
#include <vector>

#include "dependencies.h"
#include "Command.h"
#include "SceneGraphVisitor.h"
#include "Scene.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Widget;
class SceneGraphNode;
class PlayerGamepad;

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
   GetWidgetCommand(Scene::SceneGraph* scene_graph, PlayerGamepad* gamepad = nullptr);
   virtual ~GetWidgetCommand();

   void scene_graph(Scene::SceneGraph* scene_graph);
   Scene::SceneGraph* scene_graph();

   void gamepad(PlayerGamepad* gamepad);
   PlayerGamepad* gamepad();

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
   PlayerGamepad* gamepad_; // TODO: UGH

   WidgetList widgets_;   
};

#endif
