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

PreorderEntitySubscription::PreorderEntitySubscription(System* system, const std::string& id /* = "PreorderEntitySubscription" */, bool reverse_children /* = false */)
: EntitySubscription(system, id)
, reverse_children_(reverse_children)
{
   // set some messaging callbacks for when to update the entity list
   this->install_message_handler<EntityDestroyedMessage>([this](EntityDestroyedMessage& msg) {
      this->init(); // rebuild entire entity list (is there an algorithm that can do better?)
   });

   this->install_message_handler<AddToEntityMessage>([this](AddToEntityMessage& msg) {
      this->init(); // rebuild entire entity list (is there an algorithm that can do better?)
   });

   this->install_message_handler<RemoveFromEntityMessage>([this](RemoveFromEntityMessage& msg) {
      this->init(); // rebuild entire entity list (is there an algorithm that can do better?)
   });

   this->install_message_handler<ComponentAddedMessage>([this](ComponentAddedMessage& msg) {
      if (this->filter(msg.entity) && std::find(this->entities_.begin(), this->entities_.end(), msg.entity) == this->entities_.end()) {
         this->init(); // rebuild entire entity list (is there an algorithm that can do better?)
      }
   });

   this->install_message_handler<ComponentRemovedMessage>([this](ComponentRemovedMessage& msg) {
      if (this->filter(msg.entity) && std::find(this->entities_.begin(), this->entities_.end(), msg.entity) != this->entities_.end()) {
         this->init(); // rebuild entire entity list (is there an algorithm that can do better?)
      }
   });
}

PreorderEntitySubscription::~PreorderEntitySubscription() {
}

void PreorderEntitySubscription::init() {
   std::vector<Handle> entities_to_visit;
   entities_to_visit.push_back(
      this->system().root() == Handle() ? this->scene().space_handle() : this->system().root()
   );

   this->clear();

   // do a preorder traversal of scene graph
   while (!entities_to_visit.empty()) {
      Handle current = entities_to_visit.back();
      entities_to_visit.pop_back();

      Entity* e = this->scene().get_entity(current);
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

         if (this->filter(current)) {
            this->entities_.push_back(current);
         }
      }
   }
}

void PreorderEntitySubscription::add(Handle entity) {
   // empty
}

void PreorderEntitySubscription::remove(Handle entity) {
   // empty
}
