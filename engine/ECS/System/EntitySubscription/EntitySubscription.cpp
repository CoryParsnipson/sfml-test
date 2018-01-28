#include <cassert>

#include "Scene.h"
#include "System.h"
#include "EntitySubscription.h"

EntitySubscription::EntitySubscription(const std::string& id /* = "EntitySubscription" */)
: id_(id)
, break_for_each_(false)
{
}

EntitySubscription::~EntitySubscription() {
}

const std::string& EntitySubscription::id() const {
   return this->id_;
}

Scene& EntitySubscription::scene(System& system) const {
   return system.scene();
}

void EntitySubscription::break_out_of_update() {
   this->break_for_each_ = true;
}

EntityFilter& EntitySubscription::filter() {
   return this->filter_;
}

bool EntitySubscription::filter(System& system, Handle entity) {
   Entity* e = this->scene(system).get_entity(entity);
   return (e == nullptr ? false : this->filter().filter(*e));
}

void EntitySubscription::for_each(System& system, std::function<void(Handle)> entity_handler) {
   this->update_entity_list(system);

   std::vector<Handle>::const_iterator it;
   for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
      if (this->break_for_each_) {
         break;
      }

      entity_handler(*it);
   }

   // reset the break out of update loop flag
   this->break_for_each_ = false;
}
