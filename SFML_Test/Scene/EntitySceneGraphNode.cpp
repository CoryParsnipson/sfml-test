#include "EntitySceneGraphNode.h"
#include "Entity.h"

EntitySceneGraphNode::EntitySceneGraphNode(Entity& e, sf::RenderStates state /* = sf::RenderStates::Default */)
: SceneGraphNode(state)
, entity_(&e)
{
}

Entity* EntitySceneGraphNode::get_entity() {
   return this->entity_;
}

void EntitySceneGraphNode::pre_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->entity_->draw(surface, render_states);
}

void EntitySceneGraphNode::post_update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
   this->entity_->update(game, scene, entity);
}
