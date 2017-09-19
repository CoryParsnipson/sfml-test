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

   EntityFilter& filter();
   bool filter(System& system, Handle entity);

   virtual void init(System& system) = 0;
   virtual void clear() = 0;

   virtual void add(System& system, Handle entity) = 0;
   virtual void remove(Handle entity) = 0;

   virtual void for_each(System& system, std::function<void(Handle)> entity_handler) = 0;

   Scene& scene(System& system) const;

private:
   std::string id_;
   EntityFilter filter_;
};

#endif
