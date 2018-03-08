#ifndef COMPONENT_ADDED_MESSAGE_H
#define COMPONENT_ADDED_MESSAGE_H

#include <typeinfo>
#include <typeindex>

#include "Mailbox.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// ComponentAddedMessage
//
// This message is emitted whenever a component is added to an entity. This
// message does not have information about which entity or component were
// modified.
// ----------------------------------------------------------------------------
class ComponentAddedMessage : public Message {
public:
   ComponentAddedMessage(Handle entity, std::type_index component_type)
   : entity(entity)
   , component_type(component_type)
   {
   }

   template <typename ComponentT>
   bool component_is() {
      return this->component_type == std::type_index(typeid(ComponentT));
   }

   Handle entity;
   std::type_index component_type;
};

#endif
