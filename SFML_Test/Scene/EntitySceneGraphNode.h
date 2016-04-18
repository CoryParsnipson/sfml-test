#ifndef ENTITY_SCENE_GRAPH_NODE_H
#define ENTITY_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class Entity;

class EntitySceneGraphNode : public SceneGraphNode {
public:
   EntitySceneGraphNode(Entity& e, sf::RenderStates state = sf::RenderStates::Default);

   Entity* get_entity();

protected:
   Entity* entity_;

   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
   virtual void post_update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);
};

#endif
