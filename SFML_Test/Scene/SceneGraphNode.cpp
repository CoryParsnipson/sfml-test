#include "SceneGraphNode.h"

SceneGraphNode::SceneGraphNode(sf::RenderStates state /* = sf::RenderStates::Default*/, bool visible /* = true */)
: visible_(visible)
{
}

SceneGraphNode::~SceneGraphNode() {
   iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      delete *it;
   }
}

bool SceneGraphNode::visible() const {
   return this->visible_;
}

void SceneGraphNode::visible(bool visible) {
   this->visible_ = visible;
}

void SceneGraphNode::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   if (!this->visible()) {
      return;
   }

   this->pre_draw(surface, render_states);

   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }

   this->post_draw(surface, render_states);
}

void SceneGraphNode::update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
   this->pre_update(game, scene, entity);
   this->post_update(game, scene, entity);
}
