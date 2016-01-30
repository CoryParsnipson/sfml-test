#include "ReferencePart.h"

ReferencePart::ReferencePart(std::string id)
: Part(id)
{
   Service::get_logger().msg("ReferencePart", Logger::INFO, "Creating ReferencePart");
}

ReferencePart::~ReferencePart() {
}

ReferencePart::entities_t ReferencePart::get() {
   return this->references_;
}

void ReferencePart::add(Entity* entity) {
   // check for duplicates, and disallow
   entities_t::const_iterator it;
   for (it = this->references_.begin(); it != this->references_.end(); ++it) {
      if (*it == entity) {
         Service::get_logger().msg("ReferencePart", Logger::WARNING, "Ignoring request to add duplicate reference.");
         return;
      }
   }

   this->references_.push_back(entity);
}

void ReferencePart::remove(Entity* entity) {
   entities_t::const_iterator matching_entity_it = this->references_.end();
   entities_t::const_iterator it;
   for (it = this->references_.begin(); it != this->references_.end(); ++it) {
      if (*it == entity) {
         matching_entity_it = it;
         break;
      }
   }

   if (matching_entity_it != this->references_.end()) {
      this->references_.erase(matching_entity_it);
   }
} 

void ReferencePart::update(Game& game, Scene* scene, Entity* entity) {
}
