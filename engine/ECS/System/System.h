#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <memory>
#include <cassert>
#include <utility>

#include "Update.h"
#include "ObjectPool.h"
#include "BaseEntitySubscription.h"

#include "Mailbox.h"

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
{
public:
   System(const std::string& id = "System", EntitySubscription* sub = new BaseEntitySubscription());
   virtual ~System();

   void id(const std::string& id);
   const std::string& id() const;

   void enable();
   void disable();

   bool is_enabled() const;

   void init(Game& game);
   void update(Game& game);

   EntitySubscription& subscription();
   void subscription(EntitySubscription* subscription);

   Handle root() const;
   void root(Handle root);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   void receive_message(std::shared_ptr<MsgT> message);
   
   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
      typename... Args
   >
   void send_message(Args&&... args);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
      typename... Args
   >
   void send_message_async(Args&&... args);

protected:
   friend Scene& EntitySubscription::scene(System& system) const;

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

   // System interface hooks
   virtual void pre_init(Game& game) {}
   virtual void on_init(Game& game) = 0;
   virtual void post_init(Game& game) {}

   virtual void pre_update(Game& game) {}
   virtual void on_update(Game& game, Entity& e) = 0;
   virtual void post_update(Game& game) {}

   // helpers
   void send_message_helper(std::shared_ptr<Message> message);
};

// ----------------------------------------------------------------------------
// template member declarations
// ----------------------------------------------------------------------------
template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
>
void System::receive_message(std::shared_ptr<MsgT> message) {
   assert(message != nullptr);

   if (message->async()) {
      // if this message is asynchronous, process it now
      this->mailbox_.process(message);
   } else {
      // else, put the message in the mailbox queue and handle it in update
      this->mailbox_.enqueue(message);
   }
}
   
template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void System::send_message(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);
   this->send_message_helper(message);
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void System::send_message_async(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);
   message->async(true);

   this->send_message_helper(message);
}

#endif
