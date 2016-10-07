#ifndef PAN_MAP_COMMAND_H
#define PAN_MAP_COMMAND_H

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// PanMapCommand
//
// BuilderScene command to move the map and grid in the scene when the user
// right clicks on the map and drags.
// ----------------------------------------------------------------------------
class PanMapCommand : public Command {
public:
   PanMapCommand();

protected:
};

#endif
