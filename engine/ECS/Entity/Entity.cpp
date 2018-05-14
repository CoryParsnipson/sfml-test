#include <typeinfo>
#include <algorithm>

#include "Entity.h"
#include "Scene.h"

#include "EntityIdChangedMessage.h"

Entity::Entity(std::string id /* = "entity" */, Scene* scene /* = nullptr */, ComponentManager* component_manager /* = nullptr */)
: Messageable(id)
, id_(id)
, handle_()
, scene_(scene)
, components_()
, component_manager_(component_manager)
{
}

Entity::Entity(const Entity& other)
: Messageable(other)
, id_(other.id_)
, handle_(other.handle_)
, scene_(other.scene_)
, components_(other.components_)
, component_manager_(other.component_manager_)
{
}

Entity::~Entity() {
}

Entity& Entity::operator=(const Entity& other) {
   Entity tmp(other);
   this->swap(tmp);
   return *this;
}

void Entity::swap(Entity& other) {
   Messageable::swap(other);

   this->scene_ = other.scene_; // shallow copy this
   this->component_manager_ = other.component_manager_; // shallow copy this

   std::swap(this->id_, other.id_);
   std::swap(this->components_, other.components_);
}

const std::string& Entity::id() const {
   return this->id_;
}

void Entity::id(std::string id) {
   std::string old_id = this->id_;
   this->id_ = id;

   this->send_message<EntityIdChangedMessage>(this->handle(), old_id);
}

std::string Entity::to_string() {
   return "[Entity \"" + this->id() + "\"]";
}

Handle Entity::handle() const {
   return this->handle_;
}

void Entity::handle(Handle handle) {
   this->handle_ = handle;
}

void Entity::send_message_helper(MessagePtr message) {
   assert(this->scene_);
   this->scene_->receive_message(message);
}
