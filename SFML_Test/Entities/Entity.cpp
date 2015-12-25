#include "Entity.h"
#include "Part.h"

Entity::Entity() {
}

Entity::~Entity() {
   // for simplicity, the entity will assume ownership of all parts (for now)
   this->parts_.clear();
}

void Entity::add(Part* part) {
   // if part name already exists, delete it
   if (this->parts_.find(part->name()) == this->parts_.end()) {
      delete this->parts_[part->name()];
   }
   this->parts_[part->name()] = part;
}

Part* Entity::get(std::string part_name) {
   part_list_t::const_iterator it = this->parts_.find(part_name);
   if (it == this->parts_.end()) {
      return nullptr;
   }
   return it->second;
}

void Entity::update(Viewport& viewport) {
   part_list_t::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->update(*this, viewport);
   }
}
