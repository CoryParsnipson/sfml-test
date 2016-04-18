#ifndef DRAWABLE_SCENE_GRAPH_NODE_H
#define DRAWABLE_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class DrawableSceneGraphNode : public SceneGraphNode {
public:
   DrawableSceneGraphNode(Draw& d, sf::RenderStates state = sf::RenderStates::Default);

protected:
   Draw* drawable_;

   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
