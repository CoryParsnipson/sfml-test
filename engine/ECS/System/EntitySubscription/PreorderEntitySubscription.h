#ifndef PREORDER_ENTITY_SUBSCRIPTION_H
#define PREORDER_ENTITY_SUBSCRIPTION_H

#include "EntitySubscription.h"

// ----------------------------------------------------------------------------
// PreorderEntitySubscription
//
// This is an entity subscription list that uses the relationships in the
// Space component to determine which entities are part of the list.
// ----------------------------------------------------------------------------
class PreorderEntitySubscription : public EntitySubscription {
public:
   PreorderEntitySubscription(System* system, const std::string& id = "PreorderEntitySubscription", bool reverse_children = false);
   virtual ~PreorderEntitySubscription();

   virtual void init();

   virtual void add(Handle entity);
   virtual void remove(Handle entity);

   virtual void update();

private:
   bool reverse_children_;
   bool needs_cache_rebuild_;
};

#endif
