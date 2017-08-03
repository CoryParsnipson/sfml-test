#ifndef MAILBOX_H
#define MAILBOX_H

#include <map>
#include <vector>
#include <memory>
#include <cassert>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <functional>

// ----------------------------------------------------------------------------
// Message
//
// This is the base class for Mailbox messages. Any messages that need to be
// passed around from Mailbox to Mailbox has to inherit from this class.
// ----------------------------------------------------------------------------
class Message {
public:
   Message(bool async = false) : async_(async) {}
   virtual ~Message() {}

   virtual std::type_index type() const { return std::type_index(typeid(*this)); }

   void async(bool async) { this->async_ = async; }
   bool async() { return this->async_; }

private:
   bool async_;
};

#include <iostream> 
// ----------------------------------------------------------------------------
// Mailbox
//
// This class allows one to send and receive Message subtypes between other
// Mailbox instances.
// ----------------------------------------------------------------------------
class Mailbox {
public:
   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   using MessageHandlerT = std::function<void(MsgT&)>;

   using MessagePtr = std::shared_ptr<Message>;
   using Messages = std::vector<MessagePtr>;

   Mailbox(const std::string& id = "Mailbox");
   ~Mailbox();

   const std::string& id() const;
   void id(std::string id);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   void handle(MessageHandlerT<MsgT> const& handler);

   template <
      typename MsgT,
      typename LambdaT
   >
   void handle(LambdaT handler) {
      MessageHandlerT<MsgT> func = handler;
      this->handle(func);
   }

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   void process(std::shared_ptr<MsgT> message);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   void enqueue(std::shared_ptr<MsgT> message);

   void process_queue();

private:
   // ----------------------------------------------------------------------------
   // forward declarations
   // ----------------------------------------------------------------------------
   class HandlerEntry;

   // ----------------------------------------------------------------------------
   // aliases
   // ----------------------------------------------------------------------------
   using MessageHandlers = std::map<std::type_index, std::shared_ptr<HandlerEntry>>;

   // ----------------------------------------------------------------------------
   // HandlerEntry 
   //
   // This is a wrapper class that erases std::function<void(MsgT&)> types.
   // ----------------------------------------------------------------------------
   class HandlerEntry {
   public:
      virtual void operator()(Message& msg) {}
   };

   // ----------------------------------------------------------------------------
   // HandlerEntryImpl
   //
   // This actually contains the different function types
   // ----------------------------------------------------------------------------
   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
   >
   class HandlerEntryImpl : public HandlerEntry {
   public:
      HandlerEntryImpl(MessageHandlerT<MsgT> const& handler) : handler_(handler) {}

      virtual void operator()(Message& msg) {
         handler_.operator()(static_cast<MsgT&>(msg));
      }

   private:
      MessageHandlerT<MsgT> handler_;
   };

   void handle_message(MessagePtr message);

   std::string id_;
   Messages messages_;
   MessageHandlers handlers_;
};

// ----------------------------------------------------------------------------
// Mailbox method implementations
// ----------------------------------------------------------------------------
template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
>
void Mailbox::handle(MessageHandlerT<MsgT> const& handler) {
   std::shared_ptr<HandlerEntry> entry = std::make_shared<HandlerEntryImpl<MsgT>>(handler);
   this->handlers_.insert(MessageHandlers::value_type(std::type_index(typeid(MsgT)), entry));
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
>
void Mailbox::process(std::shared_ptr<MsgT> message) {
   this->handle_message(message);
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr
>
void Mailbox::enqueue(std::shared_ptr<MsgT> message) {
   assert(message != nullptr);
   this->messages_.push_back(std::move(message));
}

#endif
