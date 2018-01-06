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
   PostorderEntitySubscription(const std::string& id = "PostorderEntitySubscription", Handle root = Handle(), bool reverse_children = false);
   virtual ~PostorderEntitySubscription();

   virtual void init(System& system);
   virtual void clear();

   virtual void add(System& system, Handle entity);
   virtual void remove(Handle entity);

   virtual void for_each(System& system, std::function<void(Handle)> entity_handler);

private:
   bool reverse_children_;
   Handle root_;
};

#endif
