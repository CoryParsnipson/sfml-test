#include "EntitySceneGraphNode.h"
#include "Entity.h"

EntitySceneGraphNode::EntitySceneGraphNode(Entity& e, bool visible /* = true */)
: SceneGraphNode(visible)
, entity_(&e)
{
}

Entity* EntitySceneGraphNode::get_entity() {
   return this->entity_;
}

void EntitySceneGraphNode::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this->entity_);
}

void EntitySceneGraphNode::pre_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->entity_->draw(surface, render_states);
}

void EntitySceneGraphNode::post_update(Game& game, Scene* scene /* = nullptr */) {
   this->entity_->update(game, scene);
}
