#ifndef GET_WIDGET_COMMAND_H
#define GET_WIDGET_COMMAND_H

#include <map>
#include <vector>

#include "dependencies.h"
#include "Command.h"
#include "SceneGraphVisitor.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Widget;
class SceneGraphNode;

// ----------------------------------------------------------------------------
// aliases
// ----------------------------------------------------------------------------
using SceneGraph = std::map<int, SceneGraphNode*>;
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
   GetWidgetCommand(SceneGraph* scene_graph, sf::Vector2f target = sf::Vector2f(0, 0));
   virtual ~GetWidgetCommand();

   void scene_graph(SceneGraph* scene_graph);
   SceneGraph* scene_graph();

   void target(sf::Vector2f pos);
   const sf::Vector2f& target();

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
   sf::Vector2f target_;
   SceneGraph* scene_graph_;

   WidgetList widgets_;   
};

#endif
