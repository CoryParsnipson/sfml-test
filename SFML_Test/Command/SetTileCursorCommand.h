#ifndef SET_TILE_CURSOR_COMMAND_H
#define SET_TILE_CURSOR_COMMAND_H

#include "Command.h"
#include "UpdateTileCursorCommand.h"
#include "UpdateSelectionRectCommand.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Grid;
class Entity;

// ----------------------------------------------------------------------------
// SetTileCursorCommand
//
// Given a shared pointer to an update selection rectangle command and a tile cursor entity, update the
// tile cursor based on the selection rectangle dimensions
// ----------------------------------------------------------------------------
class SetTileCursorCommand : public Command {
public:
   SetTileCursorCommand(Grid& grid, const std::shared_ptr<UpdateSelectionRectCommand>& usr_command, Entity* tile_cursor);
   virtual ~SetTileCursorCommand();

   virtual void execute();
   virtual void unexecute();

private:
   Entity* tile_cursor_;
   std::shared_ptr<UpdateSelectionRectCommand> usr_command_;
   UpdateTileCursorCommand utc_command_;
};

#endif
