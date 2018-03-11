#include "Scene.h"
#include "System.h"
#include "BaseEntitySubscription.h"

BaseEntitySubscription::BaseEntitySubscription(System* system, const std::string& id /* = "BaseEntitySubscription" */)
: EntitySubscription(system, id)
{
}

BaseEntitySubscription::~BaseEntitySubscription() {
   this->entities_.clear();
}

void BaseEntitySubscription::init() {
   std::vector<Handle> entities_from_scene = this->scene().entities();
   for (std::vector<Handle>::iterator it = entities_from_scene.begin(); it != entities_from_scene.end(); ++it) {
      this->add(*it);
   }
}

void BaseEntitySubscription::clear() {
   this->entities_.clear();
}

void BaseEntitySubscription::add(Handle entity) {
   if (this->filter(entity)) {
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
