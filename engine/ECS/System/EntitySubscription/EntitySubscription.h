#ifndef ENTITY_SUBSCRIPTION_H
#define ENTITY_SUBSCRIPTION_H

#include <string>
#include <functional>

#include "EntityFilter.h"
#include "ObjectPool.h"

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

   virtual void init(System& system) = 0;
   virtual void clear() = 0;

   virtual void add(System& system, Handle entity) = 0;
   virtual void remove(Handle entity) = 0;

protected:
   std::vector<Handle> entities_;

private:
   std::string id_;
   bool break_for_each_;
   EntityFilter filter_;

   // this function is supposed to modify/update this->entities_
   virtual void update_entity_list(System& system) = 0;
};

#endif
