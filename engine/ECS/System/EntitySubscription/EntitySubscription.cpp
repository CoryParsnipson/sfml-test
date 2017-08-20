#include <cassert>

#include "Scene.h"
#include "EntitySubscription.h"

EntitySubscription::EntitySubscription(const std::string& id /* = "EntitySubscription" */)
: id_(id)
{
}

EntitySubscription::~EntitySubscription() {
}

const std::string& EntitySubscription::id() const {
   return this->id_;
}

EntityFilter& EntitySubscription::filter() {
   return this->filter_;
}

bool EntitySubscription::filter(Handle entity) {
   assert(this->scene_ != nullptr);

   Entity* e = this->scene_->get_entity(entity);
   return this->filter().filter(*e);
}

Scene& EntitySubscription::scene() const {
   assert(this->scene_ != nullptr);
   return *this->scene_;
}

void EntitySubscription::scene(Scene& scene) {
   this->scene_ = &scene;
}
