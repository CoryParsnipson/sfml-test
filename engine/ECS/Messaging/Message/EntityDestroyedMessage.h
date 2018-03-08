#ifndef ENTITY_DESTROYED_MESSAGE_H
#define ENTITY_DESTROYED_MESSAGE_H

#include "Mailbox.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// EntityDestroyedMessage
//
// This is a message that is sent right before an Entity is
// deallocated.
// ----------------------------------------------------------------------------
class EntityDestroyedMessage : public Message {
public:
   EntityDestroyedMessage() {}
   EntityDestroyedMessage(Handle entity) : entity(entity) {}
   
   Handle entity;
};

#endif
