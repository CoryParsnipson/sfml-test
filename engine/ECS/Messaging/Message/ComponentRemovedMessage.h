#ifndef COMPONENT_REMOVED_MESSAGE_H
#define COMPONENT_REMOVED_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// ComponentRemovedMessage
//
// This message is emitted whenever a component is removed from an entity.
// This message does not have information about which entity or component were
// modified.
// ----------------------------------------------------------------------------
class ComponentRemovedMessage : public Message {
public:
   // empty
};

#endif

