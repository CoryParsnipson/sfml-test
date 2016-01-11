#include "Map.h"

#include "Scene.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

Map::Map() {
}

Map::~Map() {
   Map::tiles_t::iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      delete *it;
   }
}

void Map::update(Scene& scene, Viewport& viewport) {
   Map::tiles_t::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      (*it)->update(scene, viewport);
   }
}

void Map::add(Entity* tile) {
   this->tiles_.push_back(tile);
}

Map::tiles_t Map::intersects(sf::Vector2i point) {
   return this->intersects(sf::FloatRect(point.x, point.y, 0, 0));
}

Map::tiles_t Map::intersects(sf::Vector2f point) {
   return this->intersects(static_cast<sf::Vector2i>(point));
}

Map::tiles_t Map::intersects(sf::FloatRect rect) {
   Map::tiles_t tiles;
   Map::tiles_t::const_iterator it;

   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>((*it)->get("physics"));
      if (tile_physics && tile_physics->intersects(rect)) {
         tiles.push_back(*it);
      }
   }

   return tiles;
}

std::string Map::to_string() {
   return "[Map]";
}
