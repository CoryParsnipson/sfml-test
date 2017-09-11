#ifndef SPATIAL_ENTITY_SUBSCRIPTION_H
#define SPATIAL_ENTITY_SUBSCRIPTION_H

#include "EntitySubscription.h"

// ----------------------------------------------------------------------------
// SpatialEntitySubscription
//
// This is an entity subscription list that uses the relationships in the
// Space component to determine which entities are part of the list.
// ----------------------------------------------------------------------------
class SpatialEntitySubscription : public EntitySubscription {
public:
   SpatialEntitySubscription(const std::string& id = "SpatialEntitySubscription");
   virtual ~SpatialEntitySubscription();

   virtual void init();
   virtual void clear();

   virtual void add(Handle entity);
   virtual void remove(Handle entity);

   virtual void for_each(std::function<void(Handle)> entity_handler);

private:
   Handle root_;
};

#endif
