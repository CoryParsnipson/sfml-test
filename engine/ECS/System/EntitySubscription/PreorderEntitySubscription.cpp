#include <cassert>
#include <algorithm>

#include "PreorderEntitySubscription.h"
#include "Scene.h"
#include "Entity.h"

#include "EntityDestroyedMessage.h"
#include "AddToEntityMessage.h"
#include "RemoveFromEntityMessage.h"
#include "ComponentAddedMessage.h"
#include "ComponentRemovedMessage.h"

PreorderEntitySubscription::PreorderEntitySubscription(const std::string& id /* = "PreorderEntitySubscription" */, bool reverse_children /* = false */)
: EntitySubscription(id)
, reverse_children_(reverse_children)
{
}

PreorderEntitySubscription::~PreorderEntitySubscription() {
}

void PreorderEntitySubscription::init(System& system) {
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
   entities_to_visit.push_back(
      system.root() == Handle() ? this->scene(system).space_handle() : system.root()
   );

   this->clear();

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
            this->entities_.push_back(current);
         }
      }
   }
}

void PreorderEntitySubscription::add(System& system, Handle entity) {
   // empty
}

void PreorderEntitySubscription::remove(Handle entity) {
   // empty
}
