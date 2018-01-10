#include <cassert>

#include "Scene.h"
#include "System.h"
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

bool EntitySubscription::filter(System& system, Handle entity) {
   Entity* e = this->scene(system).get_entity(entity);
   return (e == nullptr ? false : this->filter().filter(*e));
}

Scene& EntitySubscription::scene(System& system) const {
   return system.scene();
}
