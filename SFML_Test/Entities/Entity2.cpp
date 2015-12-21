#include "Entity2.h"
#include "Part.h"

Entity2::Entity2() {
}

Entity2::~Entity2() {
   // for simplicity, the entity will assume ownership of all parts (for now)
   this->parts_.clear();
}

void Entity2::add(Part* part) {
   this->parts_.push_back(part);
}

void Entity2::update(Viewport& viewport) {
   part_list_t::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      (*it)->update(*this, viewport);
   }
}
