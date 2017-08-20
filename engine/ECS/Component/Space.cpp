#include "Space.h"

Space::Space(const std::string& id /* = "Space Component" */)
: Component(id)
{
}

Space::~Space() {
   this->children_.clear();
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
