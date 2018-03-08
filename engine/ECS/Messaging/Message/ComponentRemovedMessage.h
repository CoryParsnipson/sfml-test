#ifndef COMPONENT_REMOVED_MESSAGE_H
#define COMPONENT_REMOVED_MESSAGE_H

#include <typeinfo>
#include <typeindex>

#include "Mailbox.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// ComponentRemovedMessage
//
// This message is emitted whenever a component is removed from an entity.
// This message does not have information about which entity or component were
// modified.
// ----------------------------------------------------------------------------
class ComponentRemovedMessage : public Message {
public:
   ComponentRemovedMessage(Handle entity, std::type_index component_type)
   : Message("ComponentRemovedMessage")
   , entity(entity)
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

