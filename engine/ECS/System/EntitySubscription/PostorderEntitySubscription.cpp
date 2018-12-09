#include <cassert>

#include "PostorderEntitySubscription.h"
#include "Scene.h"
#include "Entity.h"

#include "EntityDestroyedMessage.h"
#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

PostorderEntitySubscription::PostorderEntitySubscription(System* system, const std::string& id /* = "PostorderEntitySubscription" */, bool reverse_children /* = false */)
: EntitySubscription(system, id)
, reverse_children_(reverse_children)
{
}

PostorderEntitySubscription::~PostorderEntitySubscription() {
}

void PostorderEntitySubscription::init() {
   std::vector<Handle> entities_to_visit;
   std::vector<Handle> postorder_result;

   entities_to_visit.push_back(
      this->system().root() == Handle() ? this->scene().root_entity() : this->system().root()
   );

   this->clear();

   // do a postorder traversal of scene graph
   while (!entities_to_visit.empty()) {
      // pop top of the first "stack"
      Handle current = entities_to_visit.back();
      Entity* e = this->scene().get_entity(current);

      entities_to_visit.pop_back();

      if (e == nullptr) {
         continue;
      }
   
      SceneNode* space = e->space();

      // push to the top of the second "stack"
      postorder_result.push_back(current);

      // push children in reverse order to entities_to_visit
      for (int i = space->num_children() - 1; i >= 0; --i) {
         entities_to_visit.push_back(
            space->get_child(this->reverse_children_ ? (space->num_children() - 1 - i) : i)->entity()
         );
      }
   }

   // visit postorder_result from the end because this is supposed to be a stack
   for (std::vector<Handle>::const_reverse_iterator it = postorder_result.rbegin(); it != postorder_result.rend(); ++it) {
      if (this->filter(*it)) {
         this->entities_.push_back(*it);
      }
   }
}

void PostorderEntitySubscription::add(Handle entity) {
   // empty
}

void PostorderEntitySubscription::remove(Handle entity) {
   // empty
}
