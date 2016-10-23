#ifndef SET_TILES_COMMAND_H
#define SET_TILES_COMMAND_H

#include <vector>

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Map;
class Entity;
class SceneObject;
class Texture;

// ----------------------------------------------------------------------------
// SetTilesCommand 
//
// Given a tile cursor object and a map, add the specified tiles on the map that
// intersect with the tile cursor.
// ----------------------------------------------------------------------------
class SetTilesCommand : public Command {
public:
   SetTilesCommand(Map& map, Entity* tile_cursor, Texture& tile_texture);
   ~SetTilesCommand();

   virtual void execute();
   virtual void unexecute();

private:
   Map* map_;
   Entity* tile_cursor_;
   Texture* tile_texture_;

   std::vector<SceneObject*> added_tiles_;
};

#endif

