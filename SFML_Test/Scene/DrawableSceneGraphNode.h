#ifndef DRAWABLE_SCENE_GRAPH_NODE_H
#define DRAWABLE_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class DrawableSceneGraphNode : public SceneGraphNode {
public:
   DrawableSceneGraphNode(Draw& d, sf::RenderStates state = sf::RenderStates::Default);

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   Draw* drawable_;
};

#endif
