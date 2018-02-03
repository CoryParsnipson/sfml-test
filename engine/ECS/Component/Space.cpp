#include "Space.h"

Space::Space(const std::string& id /* = "Space Component" */)
: Component(id)
, visible_(true)
, transform_()
, parent_()
, children_()
{
}

Space::Space(const Space& other)
: Component(other.id())
, visible_(other.visible_)
, transform_(other.transform_)
, parent_(other.parent_)
, children_(other.children_)
{
}

Space::~Space() {
   this->children_.clear();
}

Space& Space::operator=(const Space& other) {
   Space tmp(other);
   this->swap(tmp);
   return *this;
}

void Space::swap(Space& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->visible_, other.visible_);
   std::swap(this->transform_, other.transform_);
   std::swap(this->parent_, other.parent_);
   std::swap(this->children_, other.children_);
}

Handle Space::parent() const {
   return this->parent_;
}

void Space::parent(Handle parent) {
   this->parent_ = parent;
}

Handle Space::add(unsigned int idx) const {
   if (idx >= this->children_.size()) {
      return Handle();
   }

   return this->children_[idx];
}

void Space::add(Handle child, int idx /* = -1 */) {
   if (idx == -1 || idx >= static_cast<signed int>(this->children_.size())) {
      this->children_.push_back(child);
   } else {
      this->children_.insert(this->children_.begin() + idx, child);
   }
}

void Space::remove(Handle child) {
   std::vector<Handle>::iterator it = this->children_.end();
   while (it > this->children_.begin()) {
      --it;
      if (child == (*it)) {
         it = this->children_.erase(it);
      }
   }
}

void Space::remove(unsigned int idx) {
   if (idx <= this->children_.size()) {
      this->children_.erase(this->children_.begin() + idx);
   }
}

Handle Space::get(unsigned int idx) const {
   if (idx < 0 || idx >= this->children_.size()) {
      return Handle();
   }
   return this->children_[idx];
}

unsigned int Space::num_children() const {
   return this->children_.size();
}

bool Space::visible() const {
   return this->visible_;
}

void Space::visible(bool visible) {
   this->visible_ = visible;
}

void Space::position(float x, float y) {
   this->transform_.setPosition(x, y);
}

void Space::position(const sf::Vector2f& position) {
   this->transform_.setPosition(position);
}

const sf::Vector2f& Space::position() const {
   return this->transform_.getPosition();
}

void Space::rotation(float angle) {
   this->transform_.setRotation(angle);
}

float Space::rotation() const {
   return this->transform_.getRotation();
}

void Space::scale(float x, float y) {
   this->transform_.setScale(x, y);
}

void Space::scale(const sf::Vector2f& factors) {
   this->transform_.setScale(factors);
}

void Space::origin(float x, float y) {
   this->transform_.setOrigin(x, y);
}

void Space::origin(const sf::Vector2f& factors) {
   this->transform_.setOrigin(factors);
}

const sf::Vector2f& Space::origin() const {
   return this->transform_.getOrigin();
}

void Space::move(float x, float y) {
   this->transform_.move(x, y);
}

void Space::move(const sf::Vector2f& offset) {
   this->transform_.move(offset);
}

void Space::rotate(float angle) {
   this->transform_.rotate(angle);
}

const sf::Transform& Space::transform() const {
   return this->transform_.getTransform();
}

const sf::Transform& Space::inverse_transform() const {
   return this->transform_.getInverseTransform();
}

std::string Space::serialize(Serializer& s) {
   // TODO: figure out solution to serializing handles?
   return "";
}

void Space::deserialize(Serializer& s, Scene& scene, std::string& d) {
   // TODO: figure out solution to serializing handles?
}
