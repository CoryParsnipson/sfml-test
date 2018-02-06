#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <string>
#include <typeindex>

#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

#include "ComponentManager.h"
#include "ObjectPool.h"
#include "Component.h"

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// Entity class
// ----------------------------------------------------------------------------
class Entity
{
public:
   using ComponentList = std::map<std::type_index, Handle>;

   Entity(const Entity& other);
   virtual ~Entity();

   const std::string& id() const;
   void id(std::string id);

   std::string to_string();
   Handle handle() const;

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

   // messaging interface
   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
      typename... Args
   >
   void send_message(Args&&... args);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
      typename... Args
   >
   void send_message_async(Args&&... args);

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

   ComponentList components_;
   ComponentManager* component_manager_;

   // helpers
   void send_message_helper(std::shared_ptr<Message> message);
   void handle(Handle handle);
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

   this->send_message_async<ComponentAddedMessage>();

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

      this->send_message_async<ComponentRemovedMessage>();
   }
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void Entity::send_message(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);
   this->send_message_helper(message);
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void Entity::send_message_async(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);
   message->async(true);

   this->send_message_helper(message);
}

#endif
