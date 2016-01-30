#include "Entity.h"

#include "Scene.h"
#include "Viewport.h"

#include "Part.h"
#include "PhysicsPart.h"
#include "GraphicsPart.h"

Entity::Entity(std::string name)
: name_(name)
{
}

Entity::~Entity() {
   // for simplicity, the entity will assume ownership of all parts (for now)
   this->parts_.clear();
}

void Entity::add(Part* part) {
   // if part name already exists, delete it
   if (this->parts_.find(part->name()) == this->parts_.end()) {
      delete this->parts_[part->name()];
   }
   this->parts_[part->name()] = part;
}

Part* Entity::get(std::string part_name) {
   part_list_t::const_iterator it = this->parts_.find(part_name);
   if (it == this->parts_.end()) {
      return nullptr;
   }
   return it->second;
}

void Entity::draw(Viewport& viewport) {
   GraphicsPart* graphics;

   part_list_t::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      if ((graphics = dynamic_cast<GraphicsPart*>(it->second))) {
         graphics->draw(viewport);
      }
   }
}

void Entity::update(Game& game, Scene* scene, Entity* entity) {
   part_list_t::const_iterator it;
   for (it = this->parts_.begin(); it != this->parts_.end(); it++) {
      it->second->update(game, scene, this);
   }
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
