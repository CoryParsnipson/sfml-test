#include <cassert>

#include "PostorderEntitySubscription.h"
#include "Scene.h"
#include "Entity.h"

#include "EntityDestroyedMessage.h"
#include "AddToEntityMessage.h"
#include "RemoveFromEntityMessage.h"
#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

PostorderEntitySubscription::PostorderEntitySubscription(const std::string& id /* = "PostorderEntitySubscription" */, bool reverse_children /* = false */)
: EntitySubscription(id)
, reverse_children_(reverse_children)
{
}

PostorderEntitySubscription::~PostorderEntitySubscription() {
}

void PostorderEntitySubscription::init(System& system) {
   // set some messaging callbacks for when to update the entity list
   this->mailbox().handle<EntityDestroyedMessage>([this, &system](EntityDestroyedMessage& msg) {
      this->init(system); // rebuild entire entity list (is there an algorithm that can do better?)
   });

   this->mailbox().handle<AddToEntityMessage>([this, &system](AddToEntityMessage& msg) {
      this->init(system); // rebuild entire entity list (is there an algorithm that can do better?)
   });

   this->mailbox().handle<RemoveFromEntityMessage>([this, &system](RemoveFromEntityMessage& msg) {
      this->init(system); // rebuild entire entity list (is there an algorithm that can do better?)
   });

   this->mailbox().handle<ComponentAddedMessage>([this, &system](ComponentAddedMessage& msg) {
      if (this->filter(system, msg.entity) && std::find(this->entities_.begin(), this->entities_.end(), msg.entity) == this->entities_.end()) {
         this->init(system); // rebuild entire entity list (is there an algorithm that can do better?)
      }
   });

   this->mailbox().handle<ComponentRemovedMessage>([this, &system](ComponentRemovedMessage& msg) {
      if (this->filter(system, msg.entity) && std::find(this->entities_.begin(), this->entities_.end(), msg.entity) != this->entities_.end()) {
         this->init(system); // rebuild entire entity list (is there an algorithm that can do better?)
      }
   });

   // now build entity list
   std::vector<Handle> entities_to_visit;
   std::vector<Handle> postorder_result;

   entities_to_visit.push_back(
      system.root() == Handle() ? this->scene(system).space_handle() : system.root()
   );

   this->clear();

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

void PostorderEntitySubscription::add(System& system, Handle entity) {
   // empty
}

void PostorderEntitySubscription::remove(Handle entity) {
   // empty
}
