#ifndef SET_GRID_VISIBILITY_MESSAGE_H
#define SET_GRID_VISIBILITY_MESSAGE_H

#include "Mailbox.h"

// ----------------------------------------------------------------------------
// SetGridVisibilityMessage
//
// This message is to let the GridSystem know to set the visibility of all
// grid elements for a specific grid component.
// ----------------------------------------------------------------------------
class SetGridVisibilityMessage : public Message {
public:
   SetGridVisibilityMessage(Handle grid_entity, bool visibility)
   : visibility(visibility)
   , grid_entity(grid_entity)
   {}

   bool visibility;
   Handle grid_entity;
};

#endif

