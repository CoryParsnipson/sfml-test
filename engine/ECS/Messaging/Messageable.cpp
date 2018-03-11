#include <stdexcept>

#include "Messageable.h"
#include "Scene.h"

Messageable::Messageable(const std::string& id)
: mailbox_(id + "Mailbox")
{
}

Messageable::Messageable(const Messageable& other)
: mailbox_(other.mailbox_)
{
}

Messageable::~Messageable() {
}

Messageable& Messageable::operator=(const Messageable& other) {
   Messageable tmp(other);
   this->swap(tmp);
   return *this;
}

void Messageable::swap(Messageable& other) {
   this->mailbox_ = other.mailbox_;
}

void Messageable::handle_queued_messages() {
   this->mailbox_.process_queue();
}

void Messageable::send_message_helper(MessagePtr message) {
   throw std::runtime_error("Children of Messageable need to implement custom send_message_helper()!");
}
