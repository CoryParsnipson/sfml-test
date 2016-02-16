#include "Entity.h"

#include "Graphics.h"
#include "Scene.h"
#include "Part.h"
#include "PhysicsPart.h"
#include "GraphicsPart.h"

Entity::Entity(std::string name)
: name_(name)
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
   if (this->parts_.find(part->name()) == this->parts_.end()) {
      delete this->parts_[part->name()];
   }
   this->parts_[part->name()] = part;
}

Part* Entity::get(std::string part_name) {
   PartList::const_iterator it = this->parts_.find(part_name);
   if (it == this->parts_.end()) {
      return nullptr;
   }
   return it->second;
}

void Entity::draw(Graphics& graphics, Layer& layer) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->draw(graphics, layer);
   }
}

void Entity::update(Game& game, Scene* scene, Entity* entity) {
   PartList::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->update(game, scene, this);
   }
}
