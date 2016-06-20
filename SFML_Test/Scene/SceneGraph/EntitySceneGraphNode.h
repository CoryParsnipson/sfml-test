#ifndef ENTITY_SCENE_GRAPH_NODE_H
#define ENTITY_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

// -----------------------------------------------------------------------------
// forward declarations
// -----------------------------------------------------------------------------
class Entity;

// -----------------------------------------------------------------------------
// EntitySceneGraphNode class
//
// This is a scene graph node that contains an entity.
// -----------------------------------------------------------------------------
class EntitySceneGraphNode : public SceneGraphNode {
public:
   EntitySceneGraphNode(Entity& e, bool visible = true);

   Entity* get_entity();

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

protected:
   Entity* entity_;

   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
   virtual void post_update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);
};

#endif