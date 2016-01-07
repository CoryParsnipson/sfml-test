#include "Map.h"

#include "Game.h"
#include "Viewport.h"
#include "Entity.h"

Map::Map() {
}

Map::~Map() {
   tiles_t::iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      delete *it;
   }
}

void Map::update(Game& game, Viewport& viewport) {
   tiles_t::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      (*it)->update(viewport);
   }
}

void Map::add(Entity* tile) {
   this->tiles_.push_back(tile);
}

std::string Map::to_string() {
   return "[Map]";
}
