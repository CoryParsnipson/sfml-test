#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <cassert>
#include <utility>

#include "Update.h"
#include "ObjectPool.h"
#include "EntityFilter.h"

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Game;
class Scene;

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
   System(const std::string& id = "System");
   virtual ~System();

   void id(const std::string& id);
   const std::string& id() const;

   EntityFilter& filter(); // get a reference to entity filter itself 
   bool filter(Entity& e) const; // use the entity filter to filter an entity

   void enable();
   void disable();

   bool is_enabled() const;

   void init(Game& game);
   void update(Game& game);
   void message();

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

private:
   std::string id_;
   bool enabled_;

   Mailbox mailbox_;
   EntityFilter filter_;
   std::vector<Handle> entities_;

   Scene* scene_;

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
