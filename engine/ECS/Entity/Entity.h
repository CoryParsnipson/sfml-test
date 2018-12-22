#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <string>
#include <typeindex>

#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

#include "SceneNode.h"

#include "ComponentManager.h"
#include "ObjectPool.h"
#include "Component.h"

#include "Messageable.h"

// ----------------------------------------------------------------------------
// Entity class
// ----------------------------------------------------------------------------
class Entity
: public Messageable
{
public:
   using ComponentList = std::map<std::type_index, Handle>;

   Entity(const Entity& other);
   virtual ~Entity();

   const std::string& id() const;
   void id(std::string id);

   SceneNode* space() const;
   void space(SceneNode* node);

   std::string to_string();
   Handle handle() const;

   void reset(); // deallocate all components from this entity

   template <
      typename ComponentType,       
      typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
   >
   ComponentType* get();

   template <
      typename ComponentType,       
      typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
   >
   void set(Handle& handle);

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
   void remove();

private:
   // only allow Scene to create entities through create_entity()
   friend class ObjectPool<Entity>;
   friend class Scene; // need this to specify the object pool allocator function

   explicit Entity(std::string id = "entity", Scene* scene = nullptr, ComponentManager* component_manager = nullptr);

   Entity& operator=(const Entity& other);
   void swap(Entity& other);

   std::string id_;
   Handle handle_;

   Scene* scene_;
   SceneNode* node_;

   ComponentList components_;
   ComponentManager* component_manager_;

   // helpers
   void handle(Handle handle);

   virtual void send_message_helper(MessagePtr message);
};

// ----------------------------------------------------------------------------
// template member specializations
// ----------------------------------------------------------------------------
template <
   typename ComponentType,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
ComponentType* Entity::get() {
   Handle handle = this->components_[std::type_index(typeid(ComponentType))];

   assert(this->component_manager_);
   return this->component_manager_->get<ComponentType>(handle);
}

template <
   typename ComponentType,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
void Entity::set(Handle& handle) {
   this->components_[std::type_index(typeid(ComponentType))] = handle;
}

template <
   typename ComponentType,
   typename... ComponentArgs,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
Handle Entity::add(ComponentArgs&&... args) {
   assert(this->component_manager_);

   // get a component from the pool and add to this entity
   Handle handle(this->component_manager_->add<ComponentType>(args...));
   this->components_[std::type_index(typeid(ComponentType))] = handle;

   this->send_message_sync<ComponentAddedMessage>(this->handle(), std::type_index(typeid(ComponentType)));

   return handle;
}

template <
   typename ComponentType,
   typename std::enable_if<std::is_base_of<Component, ComponentType>::value>::type*  = nullptr
>
void Entity::remove() {
   ComponentList::const_iterator it = this->components_.find(std::type_index(typeid(ComponentType)));
   if (it != this->components_.end()) {
      assert(this->component_manager_);

      // unattach component from this entity
      this->component_manager_->remove<ComponentType>(it->second);
      this->components_.erase(it);

      this->send_message_sync<ComponentRemovedMessage>(this->handle(), std::type_index(typeid(ComponentType)));
   }
}

#endif
