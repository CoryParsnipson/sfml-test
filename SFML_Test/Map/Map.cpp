#include "Map.h"

#include "Game.h"
#include "Viewport.h"
#include "Entity.h"

Map::Map() {
}

Map::~Map() {
   Map::tiles_t::iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      delete *it;
   }
}

void Map::update(Game& game, Viewport& viewport) {
   Map::tiles_t::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      (*it)->update(viewport);
   }
}

void Map::add(Entity* tile) {
   this->tiles_.push_back(tile);
}

Map::tiles_t Map::intersect(sf::Vector2i point) {
   return this->intersect(sf::FloatRect(point.x, point.y, 0, 0));
}

Map::tiles_t Map::intersect(sf::Vector2f point) {
   return this->intersect(static_cast<sf::Vector2i>(point));
}

Map::tiles_t Map::intersect(sf::FloatRect rect) {
   Map::tiles_t tiles;

   return tiles;
}

std::string Map::to_string() {
   return "[Map]";
}
