#include "SceneNode.h"
#include "Entity.h"

SceneNode::SceneNode()
: visible_(true)
, entity_()
, transform_()
, parent_()
, children_()
{
}

SceneNode::~SceneNode() {
}

Handle SceneNode::entity() const {
   return this->entity_;
}

void SceneNode::entity(Handle entity) {
   this->entity_ = entity;
}

SceneNode* SceneNode::parent() const {
   return this->parent_;
}

void SceneNode::parent(SceneNode* parent) {
   this->parent_ = parent;
}

void SceneNode::add(SceneNode* child, int idx /* = -1 */) {
   if (idx == -1 || idx >= static_cast<signed int>(this->children_.size())) {
      this->children_.push_back(child);
   } else {
      this->children_.insert(this->children_.begin() + idx, child);
   }
}

void SceneNode::remove(SceneNode* child) {
   std::vector<SceneNode*>::iterator it = this->children_.end();
   while (it > this->children_.begin()) {
      --it;
      if (child == (*it)) {
         it = this->children_.erase(it);
      }
   }
}

void SceneNode::remove(unsigned int idx) {
   if (idx <= this->children_.size()) {
      this->children_.erase(this->children_.begin() + idx);
   }
}

SceneNode* SceneNode::get_child(unsigned int idx) const {
   if (idx < 0 || idx >= this->children_.size()) {
      return nullptr;
   }
   return this->children_[idx];
}

unsigned int SceneNode::num_children() const {
   return this->children_.size();
}

bool SceneNode::visible() const {
   return this->visible_;
}

void SceneNode::visible(bool visible) {
   this->visible_ = visible;
}

void SceneNode::position(float x, float y) {
   this->transform_.setPosition(x, y);
}

void SceneNode::position(const sf::Vector2f& position) {
   this->transform_.setPosition(position);
}

const sf::Vector2f& SceneNode::position() const {
   return this->transform_.getPosition();
}

void SceneNode::rotation(float angle) {
   this->transform_.setRotation(angle);
}

float SceneNode::rotation() const {
   return this->transform_.getRotation();
}

void SceneNode::scale(float x, float y) {
   this->transform_.setScale(x, y);
}

void SceneNode::scale(const sf::Vector2f& scale) {
   this->transform_.setScale(scale);
}

const sf::Vector2f& SceneNode::scale() const {
   return this->transform_.getScale();
}

void SceneNode::origin(float x, float y) {
   this->transform_.setOrigin(x, y);
}

void SceneNode::origin(const sf::Vector2f& origin) {
   this->transform_.setOrigin(origin);
}

const sf::Vector2f& SceneNode::origin() const {
   return this->transform_.getOrigin();
}

void SceneNode::move(float x, float y) {
   this->transform_.move(x, y);
}

void SceneNode::move(const sf::Vector2f& offset) {
   this->transform_.move(offset);
}

void SceneNode::rotate(float angle) {
   this->transform_.rotate(angle);
}

const sf::Transform& SceneNode::transform() const {
   return this->transform_.getTransform();
}

const sf::Transform& SceneNode::inverse_transform() const {
   return this->transform_.getInverseTransform();
}
