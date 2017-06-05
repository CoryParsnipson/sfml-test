#include "Space.h"

// ----------------------------------------------------------------------------
// initialize static members
// ----------------------------------------------------------------------------
template <>
ObjectPool<Space> PooledComponent<Space>::pool("Space Component Pool", 20000);

// ----------------------------------------------------------------------------
// Space implementation
// ----------------------------------------------------------------------------
Space::Space(const std::string& id /* = "Space Component" */)
: PooledComponent<Space>(id)
, parent_(0)
{
}

Space::~Space() {
}

Space* Space::parent() {
   return Space::pool.get(this->parent_);
}

sf::Transform Space::global_transform() {
   Space* parent = this->parent();
   if (!parent) {
      return this->local_transform();
   }
   return  parent->global_transform() * this->local_transform();
}

sf::Transform Space::local_transform() {
   return this->transform_.transform;
}
