#ifndef WIDGET_SCENE_GRAPH_NODE_H
#define WIDGET_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

// -----------------------------------------------------------------------------
// forward declarations
// -----------------------------------------------------------------------------
class Widget;

// -----------------------------------------------------------------------------
// WidgetSceneGraphNode class
// 
// This is a scene graph node that contains a UI widget.
// -----------------------------------------------------------------------------
class WidgetSceneGraphNode : public SceneGraphNode {
public:
   WidgetSceneGraphNode(Widget& w, bool visible = true);

   Widget* get_widget();

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

protected:
   Widget* widget_;

   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
   virtual void post_update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);
};

#endif
