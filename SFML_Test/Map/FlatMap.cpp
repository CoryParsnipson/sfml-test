#include "FlatMap.h"

#include "Entity.h"
#include "PhysicsPart.h"

FlatMap::FlatMap() {
}

FlatMap::~FlatMap() {
}

void FlatMap::update(Game& game, Viewport& viewport) {
   map_type_t::const_iterator it;

   for (it = this->grid_.begin(); it != this->grid_.end(); it++) {
      it->second->update(viewport);
   }
}

Entity* FlatMap::set_tile(sf::Vector2i pos, Entity* tile) {
   Entity* previous_tile = nullptr;

   map_type_t::iterator it = this->grid_.find(pos);
   if (it != this->grid_.end()) {
      previous_tile = it->second;
   }
   
   // set tile physics position
   PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>(tile->get("physics"));
   if (tile_physics) {
      tile_physics->set_position(this->get_map_to_screen_pos(pos));
   } else {
      Service::get_logger().msg("FlatMap", Logger::WARNING, "Tile has no physics component.");
   }

   this->grid_[pos] = tile;
   return previous_tile;
}

sf::Vector2f FlatMap::get_map_to_screen_pos(sf::Vector2i pos) {
   sf::Vector2f screen_coord;
   
   screen_coord.x = pos.x * Settings::Instance()->TILE_WIDTH;
   screen_coord.y = pos.y * Settings::Instance()->TILE_HEIGHT;

   return screen_coord;
}

std::string FlatMap::to_string() {
   return "[FlatMap]";
}
