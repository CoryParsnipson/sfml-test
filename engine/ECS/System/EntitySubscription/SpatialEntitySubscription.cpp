#include <cassert>

#include "SpatialEntitySubscription.h"
#include "Scene.h"
#include "Entity.h"

SpatialEntitySubscription::SpatialEntitySubscription(const std::string& id /* = "SpatialEntitySubscription" */)
: EntitySubscription(id)
{
}

SpatialEntitySubscription::~SpatialEntitySubscription() {
}

void SpatialEntitySubscription::init(System& system) {
   // empty
}

void SpatialEntitySubscription::clear() {
   // empty
}

void SpatialEntitySubscription::add(System& system, Handle entity) {
   // empty
}

void SpatialEntitySubscription::remove(Handle entity) {
   // empty
}

void SpatialEntitySubscription::for_each(System& system, std::function<void(Handle)> entity_handler) {
   std::vector<Handle> entities_to_visit;
   entities_to_visit.push_back(this->scene(system).space_handle());

   // do a preorder traversal of scene graph
   while (!entities_to_visit.empty()) {
      Handle current = entities_to_visit.back();
      entities_to_visit.pop_back();

      Entity* e = this->scene(system).get_entity(current);
      if (e != nullptr) {
         Space* space = e->get<Space>();
         assert(space != nullptr);

         for (int i = space->num_children() - 1; i >= 0; --i) {
            entities_to_visit.push_back(space->get(i));
         }

         if (this->filter(system, current)) {
            entity_handler(current);
         }
      }
   }
}
