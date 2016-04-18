#ifndef ENTITY_SCENE_GRAPH_NODE_H
#define ENTITY_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class Entity;

class EntitySceneGraphNode : public SceneGraphNode {
public:
   EntitySceneGraphNode(Entity& e, sf::RenderStates state = sf::RenderStates::Default);

   Entity* get_entity();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   Entity* entity_;
};

#endif
