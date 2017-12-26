#include <typeinfo>
#include <algorithm>

#include "Entity.h"
#include "Scene.h"
#include "Part.h"
#include "PhysicsPart.h"
#include "GraphicsPart.h"

Entity::Entity(std::string id /* = "entity" */, Scene* scene /* = nullptr */, ComponentManager* component_manager /* = nullptr */)
: SceneObject(id)
, enable_debug_wireframe_(true)
, enable_debug_text_(false)
, scene_(scene)
, component_manager_(component_manager)
{
}

Entity::Entity(const Entity& other)
: SceneObject(other.id())
, enable_debug_wireframe_(other.enable_debug_wireframe_)
, enable_debug_text_(other.enable_debug_text_)
, scene_(other.scene_)
, component_manager_(other.component_manager_)
{
}

Entity::~Entity() {
   // the entity will assume ownership of all parts (for now)
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      delete it->second;
   }
   this->parts_.clear();
}

Entity& Entity::operator=(const Entity& other) {
   Entity tmp(other);
   this->swap(tmp);
   return *this;
}

void Entity::swap(Entity& other) {
   std::swap(this->enable_debug_wireframe_, other.enable_debug_wireframe_);
   std::swap(this->enable_debug_text_, other.enable_debug_text_);

   this->scene_ = other.scene_; // shallow copy this
   this->component_manager_ = other.component_manager_; // shallow copy this

   std::swap(this->parts_, other.parts_);
   std::swap(this->components_, other.components_);
}

std::string Entity::to_string() {
   std::string description = "[Entity \"" + this->id() + "\"";

   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(this->get("physics"));
   if (physics) {
      sf::Vector2f pos = physics->get_position();
      description += "@ (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
   }
   description += "]";

   return description;
}

Handle Entity::handle() const {
   return this->handle_;
}

void Entity::enable_debug_wireframe(bool debug_wireframe) {
   this->enable_debug_wireframe_ = debug_wireframe;
}

bool Entity::enable_debug_wireframe() {
   return this->enable_debug_wireframe_;
}

void Entity::enable_debug_text(bool debug_text) {
   this->enable_debug_text_ = debug_text;
}

bool Entity::enable_debug_text() {
   return this->enable_debug_text_;
}

void Entity::set_position(float x, float y) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      it->second->set_position(x, y);
   }
}

void Entity::set_position(const sf::Vector2f& pos) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      it->second->set_position(pos);
   }
}

void Entity::set_size(float width, float height) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      it->second->set_size(width, height);
   }
}

void Entity::set_size(const sf::Vector2f& size) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      it->second->set_size(size);
   }
}

void Entity::add(Part* part) {
   // if part name already exists, delete it
   this->remove(part->id());
   this->parts_[part->id()] = part;
}

void Entity::remove(const std::string& part_id) {
   PartList::iterator part_it = this->parts_.find(part_id);
   if (part_it != this->parts_.end()) {
      delete part_it->second;
      this->parts_.erase(part_it);
   }
}

Part* Entity::get(const std::string& part_id) {
   PartList::const_iterator it = this->parts_.find(part_id);
   if (it == this->parts_.end()) {
      return nullptr;
   }
   return it->second;
}

std::string Entity::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["id"] = this->id();
   data["type"] = "entity";

   PartList::iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      data[it->first] = it->second->serialize(s);
   }

   return s.serialize(data);
}

void Entity::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->id(data["id"]);

   if (data["graphics"] != "") {
      GraphicsPart* graphics_part = new GraphicsPart();
      graphics_part->deserialize(s, scene, data["graphics"]);
      this->add(graphics_part);
   }

   if (data["physics"] != "") {
      PhysicsPart* physics_part = new PhysicsPart();
      physics_part->deserialize(s, scene, data["physics"]);
      this->add(physics_part);
   }

   // TODO: create debug part if needed
   // TODO: create reference part if needed
}

bool Entity::intersects(const sf::Vector2i& other) {
   bool does_intersect = false;

   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      does_intersect |= it->second->intersects(other);
   }

   return does_intersect;
}

bool Entity::intersects(const sf::Vector2f& other) {
   bool does_intersect = false;

   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      does_intersect |= it->second->intersects(other);
   }

   return does_intersect;
}

bool Entity::intersects(const sf::FloatRect& other) {
   bool does_intersect = false;

   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      does_intersect |= it->second->intersects(other);
   }

   return does_intersect;
}

bool Entity::intersects(const SceneObject& other) {
   // TODO: implement me
   return false;
}

void Entity::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this);
}

void Entity::update(Game& game) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->update(game);
   }
}

void Entity::send_message_helper(std::shared_ptr<Message> message) {
   assert(this->scene_);
   this->scene_->handle_message(message);
}

void Entity::handle(Handle handle) {
   this->handle_ = handle;
}

void Entity::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->draw(surface, render_states);
   }
}
