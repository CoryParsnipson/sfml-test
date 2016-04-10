#include "Entity.h"
#include "Scene.h"
#include "Part.h"
#include "PhysicsPart.h"
#include "GraphicsPart.h"

Entity::Entity(std::string name)
: enable_debug_wireframe_(true)
, enable_debug_text_(false)
, name_(name)
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

std::string Entity::to_string() {
   std::string description = "[Entity \"" + this->name_ + "\"";

   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(this->get("physics"));
   if (physics) {
      sf::Vector2f pos = physics->get_position();
      description += "@ (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
   }
   description += "]";

   return description;
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

bool Entity::intersects(sf::Vector2i& other) {
   bool does_intersect = false;

   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      does_intersect |= it->second->intersects(other);
   }

   return does_intersect;
}

bool Entity::intersects(sf::Vector2f& other) {
   bool does_intersect = false;

   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      does_intersect |= it->second->intersects(other);
   }

   return does_intersect;
}

bool Entity::intersects(sf::FloatRect& other) {
   bool does_intersect = false;

   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); ++it) {
      does_intersect |= it->second->intersects(other);
   }

   return does_intersect;
}

void Entity::add(Part* part) {
   // if part name already exists, delete it
   this->remove(part->name());
   this->parts_[part->name()] = part;
}

void Entity::remove(const std::string& part_id) {
   PartList::iterator part_it = this->parts_.find(part_id);
   if (part_it != this->parts_.end()) {
      delete part_it->second;
      this->parts_.erase(part_it);
   }
}

Part* Entity::get(const std::string& part_name) {
   PartList::const_iterator it = this->parts_.find(part_name);
   if (it == this->parts_.end()) {
      return nullptr;
   }
   return it->second;
}

void Entity::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->draw(surface, render_states);
   }
}

void Entity::update(Game& game, Scene* scene, Entity* entity) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->update(game, scene, this);
   }
}

Serialize::SerialObj Entity::serialize() {
   Serialize::SerialObj obj;

   // set type tag
   obj["type"] = "entity";

   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      Serialize::SerialObj part_obj = it->second->serialize();
      obj.insert(part_obj.begin(), part_obj.end());
   }

   return obj;
}

void Entity::deserialize(Serialize::SerialObj& obj) {
   // create graphics part if needed
   if (obj["texture"] != "") {
      GraphicsPart* graphics_part = new GraphicsPart();
      graphics_part->deserialize(obj);
      this->add(graphics_part);
   }

   // create physics part if needed
   if (obj["bounds_width"] != "" && obj["bounds_height"] != "") {
      PhysicsPart* physics_part = new PhysicsPart();
      physics_part->deserialize(obj);
      this->add(physics_part);
   }

   // TODO: create debug part if needed
   // TODO: create reference part if needed
}
