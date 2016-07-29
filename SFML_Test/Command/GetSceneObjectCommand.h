#ifndef GET_WIDGET_COMMAND_H
#define GET_WIDGET_COMMAND_H

#include "dependencies.h"
#include "Command.h"
#include "SceneGraphVisitor.h"
#include "Scene.h"

// ----------------------------------------------------------------------------
// aliases
// ----------------------------------------------------------------------------
using CameraList = std::vector<Camera*>;
using EntityList = std::vector<Entity*>;
using WidgetList = std::vector<Widget*>;

using SceneObjectList = std::vector<SceneObject*>;

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class SceneObject;

// ----------------------------------------------------------------------------
// GetSceneObjectCommand class
//
// This command takes a reference to a scene graph and a coordinate. Upon
// execution, it will traverse the scene graph node and create a list of
// objects in the scene graph that contain that coordinate. This command
// will not collect objects that are not visible from the scene graph.
// ----------------------------------------------------------------------------
class GetSceneObjectCommand
: public Command
, public SceneGraphVisitor
{
public:
   GetSceneObjectCommand(SceneObject* scene_graph, sf::Vector2f target = sf::Vector2f(0, 0));
   virtual ~GetSceneObjectCommand();

   void scene_graph(SceneObject* scene_graph);
   SceneObject* scene_graph();

   const SceneObjectList& get();
   const WidgetList& get_widgets();

   void target(sf::Vector2f target);
   sf::Vector2f target();

   void clear_lists();

   // command interface
   virtual void execute();
   virtual void unexecute();

   // scene graph visitor interface
   virtual void visit(Camera*);
   virtual void visit(Entity*);
   virtual void visit(Widget*);

protected:
   sf::Vector2f target_;
   SceneObject* scene_graph_;

   CameraList cameras_;
   EntityList entities_;
   WidgetList widgets_;

   SceneObjectList scene_objects_;
};

#endif
