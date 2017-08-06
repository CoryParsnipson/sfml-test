#ifndef REMOVE_FROM_ENTITY_MESSAGE_H
#define REMOVE_FROM_ENTITY_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// RemoveFromEntityMessage
//
// This tells the Spatial System to remove the entity pointed at the child handle
// from the entity pointed at by the parent handle.
// ----------------------------------------------------------------------------
class RemoveFromEntityMessage : public Message {
public:
   Handle parent;
   Handle child;
};

#endif

