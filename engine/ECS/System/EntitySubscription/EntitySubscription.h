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
   bool filter(Handle entity);

   Scene& scene() const;
   void scene(Scene& scene);

   virtual void init() = 0;
   virtual void clear() = 0;

   virtual void add(Handle entity) = 0;
   virtual void remove(Handle entity) = 0;

   virtual void for_each(std::function<void(Handle)> entity_handler) = 0;

private:
   std::string id_;
   Scene* scene_;

   EntityFilter filter_;
};

#endif
