#ifndef SCENE_GRAPH_NODE_H
#define SCENE_GRAPH_NODE_H

#include "dependencies.h"
#include "Composite.h"
#include "Draw.h"
#include "Update.h"
#include "SceneGraphVisitor.h"

// ----------------------------------------------------------------------------
// SceneGraphNode abstract base
//
// This is a container of nodes that constitute the scene graph. There are
// different types of scene graph nodes for different objects. For instance,
// there is a CameraSceneGraphNode for scene graph nodes that hold Camera
// objects. This extra indirection step is due to the need to be able to
// store different child nodes per layer but re-use scene graph node objects
// between layers. For instance, having the same camera on layer 0 and layer 1
// but having both layers have different child nodes.
// ----------------------------------------------------------------------------
class SceneGraphNode
: public Composite<SceneGraphNode, true>
, public Draw
, public Update
{
public:
   SceneGraphNode(bool visible = true);
   virtual ~SceneGraphNode() = 0;

   SceneGraphNode* layer(int idx) const; // alias of Composite class's child() function
   int num_layers() const; // alias of Composite class's num_children() function

   // scene graph interface
   bool visible() const;
   void visible(bool visible);

   virtual sf::Transform transform();

   // mouse event interface
   virtual void on_mouse_in();
   virtual void on_mouse_out();

   virtual void on_click();
   virtual void on_release();

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor) = 0;

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

protected:
   bool visible_; // visibility for this node AND IT'S CHILDREN
   sf::Transform transform_;

   // interface hooks for children 
   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}
   virtual void post_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}

   virtual void pre_update(Game& game, Scene* scene = nullptr) {}
   virtual void post_update(Game& game, Scene* scene = nullptr) {}
};

#endif
