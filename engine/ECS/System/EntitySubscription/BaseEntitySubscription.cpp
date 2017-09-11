#include "Scene.h"
#include "BaseEntitySubscription.h"

BaseEntitySubscription::BaseEntitySubscription(const std::string& id /* = "BaseEntitySubscription" */)
: EntitySubscription(id)
{
}

BaseEntitySubscription::~BaseEntitySubscription() {
   this->entities_.clear();
}

void BaseEntitySubscription::init() {
   std::vector<Handle> entities_from_scene = this->scene().entities();
   std::for_each(entities_from_scene.begin(), entities_from_scene.end(),
      std::bind(&BaseEntitySubscription::add, this, std::placeholders::_1)
   );
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

void BaseEntitySubscription::for_each(std::function<void(Handle)> entity_handler) {
   std::for_each(this->entities_.begin(), this->entities_.end(), entity_handler);
}
