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

void EntitySceneGraphNode::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->entity_->draw(surface, render_states);

   // combine parent transform with current node's transform to pass onto children
   render_states.transform *= this->state_.transform;

   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }
}

void EntitySceneGraphNode::update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
   this->entity_->update(game, scene, entity);
}
