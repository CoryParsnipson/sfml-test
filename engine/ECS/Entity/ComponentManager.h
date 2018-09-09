#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <map>
#include <string>
#include <memory>
#include <typeinfo>
#include <typeindex>

#include "Component.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// ComponentManager
//
// This class is a holder for Object Pools of all component types.
// ----------------------------------------------------------------------------
class ComponentManager final {
public:
   using ObjectPoolBasePtr = std::unique_ptr<ObjectPoolBase>;
   using ComponentPools = std::map<std::type_index, ObjectPoolBasePtr>;

   template <
      typename ComponentType,
      typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
   >
   void create_pool(std::string pool_id = "", unsigned int size = ObjectPool<ComponentType>::default_size, ComponentType allocator = ComponentType());

   template <
      typename ComponentType,
      typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
   >
   ComponentType* get(Handle handle);

   template <
      typename ComponentType,
      typename... ComponentArgs,
      typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
   >
   Handle add(ComponentArgs&&... args);

   template <
      typename ComponentType,
      typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
   >
   void remove(Handle handle);

private:
   ComponentPools components_;
};

// ----------------------------------------------------------------------------
// template member implementations
// ----------------------------------------------------------------------------
template <
   typename ComponentType,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
void ComponentManager::create_pool(std::string pool_id /* = "" */, unsigned int size /* = ObjectPool<ComponentType>::default_size */, ComponentType allocator /* = ComponentType() */) {
   ObjectPoolBasePtr pool;
 
   if (pool_id.empty()) {
      pool_id = typeid(ComponentType).name();
   }

   this->components_[std::type_index(typeid(ComponentType))] = ObjectPoolBasePtr(new ObjectPool<ComponentType>(pool_id, size, allocator));
}

template <
   typename ComponentType,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
ComponentType* ComponentManager::get(Handle handle) {
   ObjectPoolBasePtr& pool_ptr = this->components_[std::type_index(typeid(ComponentType))];

   if (pool_ptr) {
      return static_cast<ObjectPool<ComponentType>*>(pool_ptr.get())->get(handle);
   }
   
   return nullptr;
}

template <
   typename ComponentType,
   typename... ComponentArgs,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
Handle ComponentManager::add(ComponentArgs&&... args) {
   ObjectPoolBasePtr& pool_ptr = this->components_[std::type_index(typeid(ComponentType))];

   if (!pool_ptr) {
      this->create_pool<ComponentType>();
   }

   return static_cast<ObjectPool<ComponentType>*>(pool_ptr.get())->add(args...);
}

template <
   typename ComponentType,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
void ComponentManager::remove(Handle handle) {
   ObjectPoolBasePtr& pool_ptr = this->components_[std::type_index(typeid(ComponentType))];

   if (pool_ptr) {
      static_cast<ObjectPool<ComponentType>*>(pool_ptr.get())->remove(handle);
   }
}

#endif
