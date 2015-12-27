#include "IsoMap.h"

#include "Entity.h"
#include "PhysicsPart.h"

IsoMap::IsoMap() {
}

IsoMap::~IsoMap() {
}

void IsoMap::update(Game& game, Viewport& viewport) {
   map_type_t::const_iterator it;

   for (it = this->grid_.begin(); it != this->grid_.end(); it++) {
      it->second->update(viewport);
   }
}

Entity* IsoMap::set_tile(sf::Vector2i pos, Entity* tile) {
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
      Service::get_logger().msg("IsoMap", Logger::WARNING, "Tile has no physics component.");
   }

   this->grid_[pos] = tile;
   return previous_tile;
}

Entity* IsoMap::set_tile(int x, int y, Entity* tile) {
   return this->set_tile(sf::Vector2i(x, y), tile);
}

sf::Vector2f IsoMap::get_map_to_screen_pos(sf::Vector2i pos) {
   sf::Vector2f screen_coord;

   screen_coord.x = pos.x * Settings::Instance()->TILE_WIDTH + (pos.y & 1) * (Settings::Instance()->TILE_WIDTH / 2);
   screen_coord.y = pos.y * (Settings::Instance()->TILE_HEIGHT_RHOMBUS / 2) - Settings::Instance()->TILE_HEIGHT_OVERLAP;

   return screen_coord;
}

sf::Vector2f IsoMap::get_map_to_screen_pos(int x, int y) {
   return this->get_map_to_screen_pos(sf::Vector2i(x, y));
}

std::string IsoMap::to_string() {
   return "[IsoMap]";
}
