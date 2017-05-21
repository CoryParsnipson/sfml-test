#include "Space.h"

Space::Space(const std::string& id /* = "Space Component" */)
: PooledComponent<Space>(id)
{
}

Space::~Space() {
}
