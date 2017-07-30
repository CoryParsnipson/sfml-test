#include "Mailbox.h"

// ----------------------------------------------------------------------------
// Mailbox
// ----------------------------------------------------------------------------
Mailbox::Mailbox(const std::string& id /* = "Mailbox" */)
: id_(id)
{
}

Mailbox::~Mailbox() {
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
      it->second(*message);
   }
}
