#include "SceneGraphNode.h"

SceneGraphNode::SceneGraphNode(bool visible /* = true */)
: visible_(visible)
, transform_(sf::Transform::Identity)
{
}

SceneGraphNode::~SceneGraphNode() {
   SceneGraphNode::iterator it;
   for (it = this->begin(); it != this->end(); ++it) {
      delete *it;
   }
}

SceneGraphNode* SceneGraphNode::layer(int idx) const {
   return this->child(idx);
}

int SceneGraphNode::num_layers() const {
   return this->num_children();
}

bool SceneGraphNode::visible() const {
   return this->visible_;
}

void SceneGraphNode::visible(bool visible) {
   this->visible_ = visible;
}

sf::Transform SceneGraphNode::transform() {
   sf::Transform transform = this->transform_;
   if (this->parent()) {
      transform *= this->parent()->transform();
   }

   return transform;
}

void SceneGraphNode::on_mouse_in() {}
void SceneGraphNode::on_mouse_out() {}
void SceneGraphNode::on_click() {}
void SceneGraphNode::on_release() {}

void SceneGraphNode::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   if (!this->visible()) {
      return;
   }

   render_states.transform *= this->transform_;

   this->pre_draw(surface, render_states);

   SceneGraphNode::iterator it;
   for (it = this->begin(); it != this->end(); ++it) {
      (*it)->draw(surface, render_states);
   }

   this->post_draw(surface, render_states);
}

void SceneGraphNode::update(Game& game, Scene* scene /* = nullptr */) {
   this->pre_update(game, scene);

   SceneGraphNode::iterator it;
   for (it = this->begin(); it != this->end(); ++it) {
      (*it)->update(game, scene);
   }

   this->post_update(game, scene);
}
