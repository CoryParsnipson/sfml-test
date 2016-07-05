#include "SceneGraphNode.h"

SceneGraphNode::SceneGraphNode(bool visible /* = true */)
: visible_(visible)
, transform_(sf::Transform::Identity)
, parent_(nullptr)
{
}

SceneGraphNode::~SceneGraphNode() {
   SceneGraphNode::iterator it;
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

SceneGraphNode* SceneGraphNode::parent() {
   return this->parent_;
}

void SceneGraphNode::parent(SceneGraphNode* parent) {
   if (this->parent_) {
      this->parent_->remove(this);
   }
   this->parent_ = parent;
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

   SceneGraphNode::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }

   this->post_draw(surface, render_states);
}

void SceneGraphNode::update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
   this->pre_update(game, scene, entity);

   SceneGraphNode::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->update(game, scene, entity);
   }

   this->post_update(game, scene, entity);
}

void SceneGraphNode::add_pre(SceneGraphNode* child) {
   if (child) {
      child->parent(this);
   }
}

void SceneGraphNode::remove_post(SceneGraphNode* child) {
   if (child) {
      child->parent(nullptr);
   }
}
