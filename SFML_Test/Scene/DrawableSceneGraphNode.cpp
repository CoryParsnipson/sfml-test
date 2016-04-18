#include "DrawableSceneGraphNode.h"

DrawableSceneGraphNode::DrawableSceneGraphNode(Draw& d, sf::RenderStates state /* = sf::RenderStates::Default */)
: SceneGraphNode(state)
, drawable_(&d)
{
}

void DrawableSceneGraphNode::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->drawable_->draw(surface, render_states);

   // combine parent transform with current node's transform to pass onto children
   render_states.transform *= this->state_.transform;

   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }
}

void DrawableSceneGraphNode::update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
   // empty
}
