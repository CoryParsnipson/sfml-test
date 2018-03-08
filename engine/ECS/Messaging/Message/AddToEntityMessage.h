#ifndef ADD_TO_ENTITY_MESSAGE_H
#define ADD_TO_ENTITY_MESSAGE_H

#include "Mailbox.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// AddToEntityMessage
//
// This tells the Spatial System to add the entity pointed at the child handle
// to the entity pointed at by the parent handle.
// ----------------------------------------------------------------------------
class AddToEntityMessage : public Message {
public:
   AddToEntityMessage(Handle parent, Handle child, int idx = -1)
   : Message("AddToEntityMessage")
   , parent(parent)
   , child(child)
   , idx(idx)
   {}

   Handle parent;
   Handle child;

   int idx;
};

#endif
