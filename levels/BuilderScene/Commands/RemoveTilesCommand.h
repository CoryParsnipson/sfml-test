#ifndef REMOVE_TILES_COMMAND_H
#define REMOVE_TILES_COMMAND_H

#include <vector>

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Map;
class Entity;
class SceneObject;

// ----------------------------------------------------------------------------
// RemoveTilesCommand
//
// Given a tile cursor object and a map, remove the tiles on the map that
// intersect with the tile cursor.
// ----------------------------------------------------------------------------
class RemoveTilesCommand : public Command {
public:
   RemoveTilesCommand(Map& map, Entity* tile_cursor);
   ~RemoveTilesCommand();

   virtual void execute();
   virtual void unexecute();

private:
   Map* map_;
   Entity* tile_cursor_;

   std::vector<SceneObject*> removed_tiles_;
};

#endif
