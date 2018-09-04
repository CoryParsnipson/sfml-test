#ifndef ENTITY_SUBSCRIPTION_H
#define ENTITY_SUBSCRIPTION_H

#include <string>
#include <functional>

#include "ObjectPool.h"
#include "Messageable.h"
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
class EntitySubscription
: public Messageable
{
public:
   EntitySubscription(System* system, const std::string& id);
   virtual ~EntitySubscription();

   const std::string& id() const;
   Scene& scene() const;

   void break_out_of_update();

   EntityFilter& filter();
   bool filter(Handle entity);

   void for_each(std::function<void(Handle)> entity_handler);
   void clear();

   virtual void init() = 0;
   virtual void add(Handle entity) = 0;
   virtual void remove(Handle entity) = 0;

   const std::vector<Handle>& entity_list() const;

protected:
   std::vector<Handle> entities_;

   Mailbox& mailbox();
   System& system();

private:
   std::string id_;
   bool break_for_each_;

   System* system_;
   Mailbox mailbox_;
   EntityFilter filter_;

   virtual void send_message_helper(MessagePtr message);

   virtual void on_for_each() {}
   virtual void pre_clear() {}
   virtual void post_clear() {}
};
   
#endif
