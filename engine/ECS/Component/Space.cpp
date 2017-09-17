#include "Space.h"

Space::Space(const std::string& id /* = "Space Component" */)
: Component(id)
, visible_(true)
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

sf::RenderStates& Space::states() {
   return this->transform_;
}

void Space::states(sf::RenderStates states) {
   this->transform_ = states;
}

bool Space::visible() const {
   return this->visible_;
}

void Space::visible(bool visible) {
   this->visible_ = visible;
}

std::string Space::serialize(Serializer& s) {
   // TODO: figure out solution to serializing handles?
   return "";
}

void Space::deserialize(Serializer& s, Scene& scene, std::string& d) {
   // TODO: figure out solution to serializing handles?
}
