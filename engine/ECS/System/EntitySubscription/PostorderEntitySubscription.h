#ifndef POSTORDER_ENTITY_SUBSCRIPTION_H
#define POSTORDER_ENTITY_SUBSCRIPTION_H

#include "EntitySubscription.h"

// ----------------------------------------------------------------------------
// PostorderEntitySubscription
//
// This is an entity subscription list that uses the relationships in the
// Space component to determine which entities are part of the list.
// ----------------------------------------------------------------------------
class PostorderEntitySubscription : public EntitySubscription {
public:
   PostorderEntitySubscription(System* system, const std::string& id = "PostorderEntitySubscription", bool reverse_children = false);
   virtual ~PostorderEntitySubscription();

   virtual void init();

   virtual void add(Handle entity);
   virtual void remove(Handle entity);

private:
   bool reverse_children_;
};

#endif
