#include "SceneGraphNode.h"

SceneGraphNode::SceneGraphNode(sf::RenderStates state /* = sf::RenderStates::Default*/)
: state_(state)
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
