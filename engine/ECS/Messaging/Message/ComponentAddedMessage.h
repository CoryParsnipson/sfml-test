#ifndef COMPONENT_ADDED_MESSAGE_H
#define COMPONENT_ADDED_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// ComponentAddedMessage
//
// This message is emitted whenever a component is added to an entity. This
// message does not have information about which entity or component were
// modified.
// ----------------------------------------------------------------------------
class ComponentAddedMessage : public Message {
public:
   // empty
};

#endif
