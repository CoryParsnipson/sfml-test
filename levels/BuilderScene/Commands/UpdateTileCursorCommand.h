#ifndef UPDATE_TILE_CURSOR_COMMAND_H
#define UPDATE_TILE_CURSOR_COMMAND_H

#include "sfml.h"

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Grid;
class Entity;

// ----------------------------------------------------------------------------
// UpdateTileCursorCommand
//
// Update a given tile cursor entity.
// ----------------------------------------------------------------------------
class UpdateTileCursorCommand : public Command {
public:
   UpdateTileCursorCommand(Grid& grid, Entity* tile_cursor);
   virtual ~UpdateTileCursorCommand();

   virtual void execute();
   virtual void unexecute();

   void click_pos(sf::Vector2f pos);
   void click_pos(float x, float y);
   const sf::Vector2f& click_pos();

   void release_pos(sf::Vector2f pos);
   void release_pos(float x, float y);
   const sf::Vector2f& release_pos();

private:
   Grid* grid_;
   Entity* tile_cursor_;

   sf::Vector2f click_pos_;
   sf::Vector2f release_pos_;
};

#endif
