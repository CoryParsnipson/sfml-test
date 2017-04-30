#ifndef COMPONENT1_H
#define COMPONENT1_H

#include "PooledComponent.h"

// -----------------------------------------------------------------------------
// Test component
// -----------------------------------------------------------------------------
class Component1 : public PooledComponent<Component1> {
public:
   explicit Component1(const std::string& id = "Component1")
   : PooledComponent<Component1>(id)
   {
   }

   virtual void init() {}
   virtual void reset() {}
};

// -----------------------------------------------------------------------------
// initialize component pool
// -----------------------------------------------------------------------------
template <>
ObjectPool<Component1> PooledComponent<Component1>::pool("Component1 Pool", 10000);

#endif
