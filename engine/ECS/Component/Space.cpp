#include "Space.h"

// ----------------------------------------------------------------------------
// initialize static members
// ----------------------------------------------------------------------------
template <>
ObjectPool<Space> PooledComponent<Space>::pool("Circle Component Pool", 20000);

// ----------------------------------------------------------------------------
// Space implementation
// ----------------------------------------------------------------------------
Space::Space(const std::string& id /* = "Space Component" */)
: PooledComponent<Space>(id)
{
}

Space::~Space() {
}
