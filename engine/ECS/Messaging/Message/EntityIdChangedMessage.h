#ifndef ENTITY_ID_CHANGED_MESSAGE_H
#define ENTITY_ID_CHANGED_MESSAGE_H

#include "Mailbox.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// EntityIdChangedMessage 
//
// This is a message that is sent when an Entity's id is changed.
// ----------------------------------------------------------------------------
class EntityIdChangedMessage : public Message {
public:
   EntityIdChangedMessage(Handle entity, const std::string& old_id)
   : Message("EntityIdChangedMessage")
   , entity(entity)
   , old_id(old_id)
   {}
   
   Handle entity;
   std::string old_id;
};

#endif
