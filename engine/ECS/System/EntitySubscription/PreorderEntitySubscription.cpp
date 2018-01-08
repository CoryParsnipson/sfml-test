#include <cassert>

#include "PreorderEntitySubscription.h"
#include "Scene.h"
#include "Entity.h"

PreorderEntitySubscription::PreorderEntitySubscription(const std::string& id /* = "PreorderEntitySubscription" */, bool reverse_children /* = false */)
: EntitySubscription(id)
, reverse_children_(reverse_children)
{
}

PreorderEntitySubscription::~PreorderEntitySubscription() {
}

void PreorderEntitySubscription::init(System& system) {
   // empty
}

void PreorderEntitySubscription::clear() {
   // empty
}

void PreorderEntitySubscription::add(System& system, Handle entity) {
   // empty
}

void PreorderEntitySubscription::remove(Handle entity) {
   // empty
}

void PreorderEntitySubscription::for_each(System& system, std::function<void(Handle)> entity_handler) {
   std::vector<Handle> entities_to_visit;
   entities_to_visit.push_back(
      system.root() == Handle() ? this->scene(system).space_handle() : system.root()
   );

   // do a preorder traversal of scene graph
   while (!entities_to_visit.empty()) {
      Handle current = entities_to_visit.back();
      entities_to_visit.pop_back();

      Entity* e = this->scene(system).get_entity(current);
      if (e != nullptr) {
         Space* space = e->get<Space>();
         assert(space != nullptr);

         if (this->reverse_children_) {
            for (unsigned int i = 0; i < space->num_children(); ++i) {
               entities_to_visit.push_back(space->get(i));
            }
         } else {
            for (int i = space->num_children() - 1; i >= 0; --i) {
               entities_to_visit.push_back(space->get(i));
            }
         }

         if (this->filter(system, current)) {
            entity_handler(current);
         }
      }
   }
}
