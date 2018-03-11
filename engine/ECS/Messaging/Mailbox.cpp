#include "Mailbox.h"

// ----------------------------------------------------------------------------
// Mailbox
// ----------------------------------------------------------------------------
Mailbox::Mailbox(const std::string& id /* = "Mailbox" */)
: id_(id)
, messages_()
, handlers_()
{
}

Mailbox::Mailbox(const Mailbox& other)
: id_(other.id_)
, messages_(other.messages_)
, handlers_(other.handlers_)
{
}

Mailbox::~Mailbox() {
}

Mailbox& Mailbox::operator=(const Mailbox& other) {
   Mailbox tmp(other);
   this->swap(tmp);
   return *this;
}

void Mailbox::swap(Mailbox& other) {
   std::swap(this->id_, other.id_);
   std::swap(this->messages_, other.messages_);
   std::swap(this->handlers_, other.handlers_);
}

const std::string& Mailbox::id() const {
   return this->id_;
}

void Mailbox::id(std::string id) {
   this->id_ = id;
}

void Mailbox::process_queue() {
   for (Messages::const_iterator it = this->messages_.begin(); it != this->messages_.end(); ++it) {
      this->handle_message(*it);
   }

   this->messages_.clear();
}

void Mailbox::handle_message(MessagePtr message) {
   MessageHandlers::iterator it = this->handlers_.find(message->type());
   if (it != this->handlers_.end()) {
      (*it->second)(*message);
   }
}
