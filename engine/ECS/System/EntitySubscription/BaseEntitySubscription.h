#ifndef BASE_ENTITY_SUBSCRIPTION_H
#define BASE_ENTITY_SUBSCRIPTION_H

#include <vector>

#include "EntitySubscription.h"

// ----------------------------------------------------------------------------
// BaseEntitySubscription
//
// This is a "default" entity subscription that is basically just a linear
// list that contains all entities that match the filter criteria.
// ----------------------------------------------------------------------------
class BaseEntitySubscription : public EntitySubscription {
public:
   BaseEntitySubscription(System* system, const std::string& id = "BaseEntitySubscription");
   virtual ~BaseEntitySubscription();

   virtual void init();
   virtual void clear();

   virtual void add(Handle entity);
   virtual void remove(Handle entity);
};

#endif
