#ifndef ADD_TO_ENTITY_MESSAGE_H
#define ADD_TO_ENTITY_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// AddToEntityMessage
//
// This tells the Spatial System to add the entity pointed at the child handle
// to the entity pointed at by the parent handle.
// ----------------------------------------------------------------------------
class AddToEntityMessage : public Message {
public:
   AddToEntityMessage() {}
   AddToEntityMessage(Handle parent, Handle child) : parent(parent), child(child) {}

   Handle parent;
   Handle child;
};

#endif
