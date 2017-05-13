#ifndef COMPONENT2_H
#define COMPONENT2_H

#include "PooledComponent.h"

// -----------------------------------------------------------------------------
// Test component
// -----------------------------------------------------------------------------
class Component2 : public PooledComponent<Component2> {
public:
   explicit Component2(const std::string& id = "Component2")
   : PooledComponent<Component2>(id)
   {
   }

   virtual void init() {}
   virtual void reset() {}
};

// -----------------------------------------------------------------------------
// initialize component pool
// -----------------------------------------------------------------------------
template <>
ObjectPool<Component2> PooledComponent<Component2>::pool("Component2 Pool", 10000);

#endif

