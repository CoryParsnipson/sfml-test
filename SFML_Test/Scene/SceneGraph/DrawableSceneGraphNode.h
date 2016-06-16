#ifndef DRAWABLE_SCENE_GRAPH_NODE_H
#define DRAWABLE_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class DrawableSceneGraphNode : public SceneGraphNode {
public:
   DrawableSceneGraphNode(Draw& d, bool visible = true);

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

protected:
   Draw* drawable_;

   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
