#include "SceneGraphNode.h"

SceneGraphNode::SceneGraphNode(Draw* drawable /* = nullptr */, sf::RenderStates state /* = sf::RenderStates::Default*/)
: state_(state)
, drawable_(drawable)
{
}

SceneGraphNode::~SceneGraphNode() {
   iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      delete *it;
   }
}

void SceneGraphNode::set_render_state(const sf::RenderStates& state) {
   this->state_ = state;
}

const sf::RenderStates& SceneGraphNode::get_render_state() {
   return this->state_;
}

void SceneGraphNode::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   // combine parent transform with current node's transform to pass onto children
   render_states.transform *= this->state_.transform;

   if (this->drawable_) {
      this->drawable_->draw(surface, render_states);
   }

   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }
}
