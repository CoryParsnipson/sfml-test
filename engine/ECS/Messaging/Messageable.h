#ifndef MESSAGEABLE_H
#define MESSAGEABLE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// Messageable
//
// This is an interface for classes that need to be able to send or receive
// messages.
// ----------------------------------------------------------------------------
class Messageable {
public:
   Messageable(const std::string& id);
   Messageable(const Messageable& other);
   virtual ~Messageable();

   Messageable& operator=(const Messageable& other);
   void swap(Messageable& other);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   void receive_message(std::shared_ptr<MsgT> message);

protected:
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
   void send_message_sync(Args&&... args);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   void install_message_handler(const Mailbox::MessageHandlerT<MsgT>& handler);

   template <
      typename MsgT,
      typename LambdaT
   >
   void install_message_handler(LambdaT handler);

   void handle_queued_messages();

private:
   Mailbox mailbox_;

   virtual void send_message_helper(MessagePtr message);

   virtual void pre_send_message(MessagePtr message) {}
   virtual void post_send_message(MessagePtr message) {}

   virtual void pre_receive_message(MessagePtr message) {}
   virtual void post_receive_message(MessagePtr message) {}
};

// ----------------------------------------------------------------------------
// template member definitions
// ----------------------------------------------------------------------------
template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
>
void Messageable::receive_message(std::shared_ptr<MsgT> message) {
   assert(message != nullptr);

   this->pre_receive_message(message);

   if (message->async()) {
      // if this message is asynchronous, process it now
      this->mailbox_.process(message);
   } else {
      // else, put the message in the mailbox queue and handle it in update
      this->mailbox_.enqueue(message);
   }

   this->post_receive_message(message);
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void Messageable::send_message(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);
   message->async(true);

   this->pre_send_message(message);
   this->send_message_helper(message);
   this->post_send_message(message);
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void Messageable::send_message_sync(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);

   this->pre_send_message(message);
   this->send_message_helper(message);
   this->post_send_message(message);
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
>
void Messageable::install_message_handler(const Mailbox::MessageHandlerT<MsgT>& handler) {
   this->mailbox_.handle<MsgT>(handler);
}

template <
   typename MsgT,
   typename LambdaT
>
void Messageable::install_message_handler(LambdaT handler) {
   this->mailbox_.handle<MsgT>(handler);
}

#endif
