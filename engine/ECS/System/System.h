#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <memory>
#include <cassert>
#include <utility>
#include <SFML/Graphics.hpp>

#include "Update.h"
#include "ObjectPool.h"
#include "BaseEntitySubscription.h"

#include "Messageable.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Game;
class Scene;
class EntityFilter;

// ----------------------------------------------------------------------------
// System
//
// This is the base class for anything that needs to operate on and/or
// between Entities and Components.
// ----------------------------------------------------------------------------
class System
: public Update
, public Messageable
{
public:
   System(const std::string& id = "System", EntitySubscription* sub = nullptr);
   virtual ~System();

   void id(const std::string& id);
   const std::string& id() const;

   void enable();
   void disable();

   bool is_enabled() const;

   void init(Game& game);
   void update(Game& game);

   void break_out_of_update();

   EntitySubscription& subscription();
   void subscription(EntitySubscription* subscription);

   Handle root() const;
   void root(Handle root);

protected:
   friend Scene& EntitySubscription::scene() const;

   Scene& scene();
   Mailbox& mailbox();

   EntityFilter& subscribe_to();

   sf::Transform local_transform(Entity& e);
   sf::Transform global_transform(Entity& e);

   bool is_visible(Handle entity);

private:
   std::string id_;
   bool enabled_;

   Scene* scene_;
   Handle root_;

   Mailbox mailbox_;
   EntitySubscription* subscription_;

   virtual void send_message_helper(MessagePtr message);

   virtual void post_receive_message(MessagePtr message);

   // System interface hooks
   virtual void pre_init(Game& game) {}
   virtual void on_init(Game& game) = 0;
   virtual void post_init(Game& game) {}

   virtual void pre_update(Game& game) {}
   virtual void on_update(Game& game, Entity& e) = 0;
   virtual void post_update(Game& game) {}
};

#endif
