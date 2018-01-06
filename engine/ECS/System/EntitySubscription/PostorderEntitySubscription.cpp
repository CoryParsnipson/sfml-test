#include <cassert>

#include "PostorderEntitySubscription.h"
#include "Scene.h"
#include "Entity.h"

PostorderEntitySubscription::PostorderEntitySubscription(const std::string& id /* = "PostorderEntitySubscription" */, Handle root /* = Handle() */, bool reverse_children /* = false */)
: EntitySubscription(id)
, reverse_children_(reverse_children)
, root_(root)
{
}

PostorderEntitySubscription::~PostorderEntitySubscription() {
}

void PostorderEntitySubscription::init(System& system) {
   // empty
}

void PostorderEntitySubscription::clear() {
   // empty
}

void PostorderEntitySubscription::add(System& system, Handle entity) {
   // empty
}

void PostorderEntitySubscription::remove(Handle entity) {
   // empty
}

void PostorderEntitySubscription::for_each(System& system, std::function<void(Handle)> entity_handler) {
   std::vector<Handle> entities_to_visit;
   std::vector<Handle> postorder_result;

   if (!this->scene(system).get_entity(this->root_)) {
      entities_to_visit.push_back(this->scene(system).space_handle());
   } else {
      entities_to_visit.push_back(this->root_);
   }

   // do a postorder traversal of scene graph
   while (!entities_to_visit.empty()) {
      // pop top of the first "stack"
      Handle current = entities_to_visit.back();
      Entity* e = this->scene(system).get_entity(current);

      entities_to_visit.pop_back();

      if (e == nullptr) {
         continue;
      }
   
      Space* space = e->get<Space>();

      // push to the top of the second "stack"
      postorder_result.push_back(current);

      // push children in reverse order to entities_to_visit
      for (int i = space->num_children() - 1; i >= 0; --i) {
         entities_to_visit.push_back(
            space->get(this->reverse_children_ ? (space->num_children() - 1 - i) : i)
         );
      }
   }

   for (std::vector<Handle>::const_iterator it = postorder_result.begin(); it != postorder_result.end(); ++it) {
      if (this->filter(system, *it)) {
         entity_handler(*it);
      }
   }
}
