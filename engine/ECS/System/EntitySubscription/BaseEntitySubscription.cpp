#include "Scene.h"
#include "System.h"
#include "BaseEntitySubscription.h"

BaseEntitySubscription::BaseEntitySubscription(const std::string& id /* = "BaseEntitySubscription" */)
: EntitySubscription(id)
{
}

BaseEntitySubscription::~BaseEntitySubscription() {
   this->entities_.clear();
}

void BaseEntitySubscription::init(System& system) {
   std::vector<Handle> entities_from_scene = this->scene(system).entities();
   for (std::vector<Handle>::iterator it = entities_from_scene.begin(); it != entities_from_scene.end(); ++it) {
      this->add(system, *it);
   }
}

void BaseEntitySubscription::clear() {
   this->entities_.clear();
}

void BaseEntitySubscription::add(System& system, Handle entity) {
   if (this->filter(system, entity)) {
      this->entities_.push_back(entity);
   }
}

void BaseEntitySubscription::remove(Handle entity) {
   std::vector<Handle>::iterator it = this->entities_.end();
   while (it > this->entities_.begin()) {
      --it;
      if (entity == (*it)) {
         it = this->entities_.erase(it);
      }
   }
}

void BaseEntitySubscription::for_each(System& system, std::function<void(Handle)> entity_handler) {
   std::for_each(this->entities_.begin(), this->entities_.end(), entity_handler);
}
