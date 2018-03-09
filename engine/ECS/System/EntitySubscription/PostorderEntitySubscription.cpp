#include <cassert>

#include "PostorderEntitySubscription.h"
#include "Scene.h"
#include "Entity.h"

PostorderEntitySubscription::PostorderEntitySubscription(const std::string& id /* = "PostorderEntitySubscription" */, bool reverse_children /* = false */)
: EntitySubscription(id)
, reverse_children_(reverse_children)
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

void PostorderEntitySubscription::on_for_each(System& system) {
   std::vector<Handle> entities_to_visit;
   std::vector<Handle> postorder_result;

   entities_to_visit.push_back(
      system.root() == Handle() ? this->scene(system).space_handle() : system.root()
   );

   // clear entity list (this subscription list implementation is not optimized)
   this->entities_.clear();

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

   // visit postorder_result from the end because this is supposed to be a stack
   for (std::vector<Handle>::const_reverse_iterator it = postorder_result.rbegin(); it != postorder_result.rend(); ++it) {
      if (this->filter(system, *it)) {
         this->entities_.push_back(*it);
      }
   }
}
