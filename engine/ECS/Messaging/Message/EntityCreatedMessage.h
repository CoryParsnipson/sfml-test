#ifndef ENTITY_CREATED_MESSAGE_H
#define ENTITY_CREATED_MESSAGE_H

#include "Mailbox.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// EntityCreatedMessage
//
// This is a message that is sent when a new Entity is allocated.
// ----------------------------------------------------------------------------
class EntityCreatedMessage : public Message {
public:
   EntityCreatedMessage() {}
   EntityCreatedMessage(Handle entity) : entity(entity) {}
   
   Handle entity;
};

#endif
