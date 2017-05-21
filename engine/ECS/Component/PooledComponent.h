#ifndef POOLED_COMPONENT_H
#define POOLED_COMPONENT_H

#include <typeinfo>

#include "Component.h"
#include "ObjectPool.h"

// -----------------------------------------------------------------------------
// PooledComponent
//
// This is a Component (abstract) subclass that keeps all instances of the
// class in a shared object pool. Note, the intended use of this class (and its
// children) is to not call the constructor explicitly, but to obtain objects
// from the automatically provided static object pool.
// -----------------------------------------------------------------------------
template <typename PooledObjectType>
class PooledComponent : public Component {
public:
   static ObjectPool<PooledObjectType> pool;

   explicit PooledComponent(const std::string& id = "PooledComponent");
   virtual ~PooledComponent() {};
};

// -----------------------------------------------------------------------------
// PooledComponent method implementations
// -----------------------------------------------------------------------------
template <typename PooledObjectType>
PooledComponent<PooledObjectType>::PooledComponent(const std::string& id /* = "PooledComponent" */)
: Component(id)
{
}

#endif
