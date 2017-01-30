#include "SetTilesCommand.h"
#include "Game.h"
#include "Map.h"
#include "Grid.h"
#include "Entity.h"
#include "Texture.h"
#include "TileFactory.h"
#include "PhysicsPart.h"

SetTilesCommand::SetTilesCommand(Map& map, Entity* tile_cursor, Texture& tile_texture)
: map_(&map)
, tile_cursor_(tile_cursor)
, tile_texture_(&tile_texture)
{
}

SetTilesCommand::~SetTilesCommand() {
}

void SetTilesCommand::execute() {
   if (!this->map_ || !this->tile_cursor_ || !this->tile_cursor_->visible()) {
      return;
   }

   PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(this->tile_cursor_->get("physics"));
   if (!tc_physics) {
      Game::logger().msg(this->id_, Logger::ERROR, "Tile cursor does not have physics part!");
      return;
   }

   sf::FloatRect bounds = tc_physics->get_bounding_box();
   for (int tile_col = bounds.left; tile_col < bounds.left + bounds.width; tile_col += this->map_->grid()->tile_width()) {
      for (int tile_row = bounds.top; tile_row < bounds.top + bounds.height; tile_row += this->map_->grid()->tile_height()) {
         Entity* tile = TileFactory::inst()->create_tile(
            *this->tile_texture_,
            sf::Vector2f(tile_col, tile_row),
            false
         );
         tile->id("tile_" + std::to_string(tile_col / this->map_->grid()->tile_height()) + "x" + std::to_string(tile_row / this->map_->grid()->tile_width()));

         this->map_->add(tile);
      }
   }
}

void SetTilesCommand::unexecute() {
   if (!this->map_) {
      return;
   }

   std::vector<SceneObject*>::const_iterator it;
   for (it = this->added_tiles_.begin(); it != this->added_tiles_.end(); ++it) {
      this->map_->remove(*it);
   }

   this->added_tiles_.clear();
}
