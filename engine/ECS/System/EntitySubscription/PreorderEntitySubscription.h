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
   PreorderEntitySubscription(const std::string& id = "PreorderEntitySubscription", bool reverse_children = false);
   virtual ~PreorderEntitySubscription();

   virtual void init(System& system);
   virtual void clear();

   virtual void add(System& system, Handle entity);
   virtual void remove(Handle entity);

private:
   bool reverse_children_;

   virtual void on_for_each(System& system);
};

#endif
