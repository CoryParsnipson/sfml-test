#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <string>
#include <typeindex>

#include "sfml.h"

#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

#include "ComponentManager.h"
#include "ObjectPool.h"
#include "Component.h"

#include "SceneObject.h"
#include "Serializable.h"

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// forward declarations 
// ----------------------------------------------------------------------------
class Part;

// ----------------------------------------------------------------------------
// Entity class
// ----------------------------------------------------------------------------
class Entity
: public SceneObject
{
public:
   using PartList = std::map<std::string, Part*>;
   using ComponentList = std::map<std::type_index, Handle>;

   // TODO: make this private (so only the object pool can make them) when Entity conversion is done
   explicit Entity(std::string id = "entity", Scene* scene = nullptr, ComponentManager* component_manager = nullptr);
   Entity(const Entity& other);
   virtual ~Entity();

   Entity& operator=(const Entity& other);
   void swap(Entity& other);

   std::string to_string();

   Handle handle() const;

   void enable_debug_wireframe(bool debug_wireframe);
   bool enable_debug_wireframe();

   void enable_debug_text(bool debug_text);
   bool enable_debug_text();

   void set_position(float x, float y);
   void set_position(const sf::Vector2f& pos);

   void set_size(float width, float height);
   void set_size(const sf::Vector2f& size);

   // part management interface
   void add(Part* part);
   void remove(const std::string& part_id);
   Part* get(const std::string& part_id);

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

   // serializable interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

   // scene graph interface
   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);
   virtual bool intersects(const SceneObject& other);

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

   // update interface
   virtual void update(Game& game);

private:
   // allow Scene class to create entities
   friend class ObjectPool<Entity>;
   friend class Scene;

   bool enable_debug_wireframe_;
   bool enable_debug_text_;

   Scene* scene_;
   Handle handle_;
   ComponentManager* component_manager_;

   PartList parts_;
   ComponentList components_;

   // helpers
   void send_message_helper(std::shared_ptr<Message> message);
   void handle(Handle handle);

   // scene graph interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
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
