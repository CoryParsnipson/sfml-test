#include "RemoveTilesCommand.h"
#include "Game.h"
#include "SceneObject.h"
#include "Map.h"
#include "Entity.h"
#include "PhysicsPart.h"

RemoveTilesCommand::RemoveTilesCommand(Map& map, Entity* tile_cursor)
: map_(&map)
, tile_cursor_(tile_cursor)
{
}

RemoveTilesCommand::~RemoveTilesCommand() {
   this->removed_tiles_.clear();
}

void RemoveTilesCommand::execute() {
   if (!this->tile_cursor_) {
      return;
   }

   this->removed_tiles_.clear();
   
   // get bounds of tile cursor
   PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(this->tile_cursor_->get("physics"));
   if (!tc_physics) {
      Game::logger().msg(this->id_, Logger::ERROR, "Tile cursor does not have physics part!");
      return;
   }

   Map::iterator it;
   for (it = this->map_->begin(); it != this->map_->end(); ++it) {
      if ((*it)->intersects(tc_physics->get_bounding_box())) {
         this->removed_tiles_.push_back(*it);
         this->map_->remove(*it);
      }
   }
}

void RemoveTilesCommand::unexecute() {
   for (std::vector<SceneObject*>::iterator it = this->removed_tiles_.begin(); it != this->removed_tiles_.end(); ++it) {
      this->map_->add(*it);
   }

   this->removed_tiles_.clear();
}
