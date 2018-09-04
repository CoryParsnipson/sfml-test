#include <cassert>

#include "Scene.h"
#include "System.h"
#include "EntitySubscription.h"

EntitySubscription::EntitySubscription(System* system, const std::string& id)
: Messageable(id)
, id_(id)
, break_for_each_(false)
, system_(system)
, mailbox_(id + "Mailbox")
, filter_(id + "EntityFilter")
{
}

EntitySubscription::~EntitySubscription() {
}

const std::string& EntitySubscription::id() const {
   return this->id_;
}

Scene& EntitySubscription::scene() const {
   assert(this->system_);
   return this->system_->scene();
}

void EntitySubscription::break_out_of_update() {
   this->break_for_each_ = true;
}

EntityFilter& EntitySubscription::filter() {
   return this->filter_;
}

bool EntitySubscription::filter(Handle entity) {
   Entity* e = this->scene().get_entity(entity);
   return (e == nullptr ? false : this->filter().filter(*e));
}

void EntitySubscription::for_each(std::function<void(Handle)> entity_handler) {
   this->on_for_each();

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

void EntitySubscription::clear() {
   this->pre_clear();
   this->entities_.clear();
   this->post_clear();
}

const std::vector<Handle>& EntitySubscription::entity_list() const {
   return this->entities_;
}

Mailbox& EntitySubscription::mailbox() {
   return this->mailbox_;
}

System& EntitySubscription::system() {
   assert(this->system_);
   return *this->system_;
}

void EntitySubscription::send_message_helper(MessagePtr message) {
   assert(this->system_);
   this->scene().receive_message(message);
}
