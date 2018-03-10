#ifndef ENTITY_SUBSCRIPTION_H
#define ENTITY_SUBSCRIPTION_H

#include <string>
#include <functional>

#include "Mailbox.h"
#include "ObjectPool.h"
#include "EntityFilter.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Game;
class Scene;
class System;

// ----------------------------------------------------------------------------
// EntitySubscription
//
// Base class for Entity Subscriptions used by Systems.
// ----------------------------------------------------------------------------
class EntitySubscription {
public:
   EntitySubscription(const std::string& id = "EntitySubscription");
   virtual ~EntitySubscription();

   const std::string& id() const;
   Scene& scene(System& system) const;

   void break_out_of_update();

   EntityFilter& filter();
   bool filter(System& system, Handle entity);

   void for_each(System& system, std::function<void(Handle)> entity_handler);
   void clear();

   virtual void init(System& system) = 0;
   virtual void add(System& system, Handle entity) = 0;
   virtual void remove(Handle entity) = 0;

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   void receive_message(std::shared_ptr<MsgT> message);
   
protected:
   std::vector<Handle> entities_;

   Mailbox& mailbox();

private:
   std::string id_;
   bool break_for_each_;

   Mailbox mailbox_;
   EntityFilter filter_;

   virtual void on_for_each(System& system) {}
   virtual void pre_clear() {}
   virtual void post_clear() {}
};

// ----------------------------------------------------------------------------
// template member declarations
// ----------------------------------------------------------------------------
template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
>
void EntitySubscription::receive_message(std::shared_ptr<MsgT> message) {
   assert(message != nullptr);

   if (message->async()) {
      // if this message is asynchronous, process it now
      this->mailbox_.process(message);
   } else {
      // else, put the message in the mailbox queue and handle it in update
      this->mailbox_.enqueue(message);
   }
}
   
#endif
