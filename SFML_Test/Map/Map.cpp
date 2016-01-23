#include "Map.h"

#include "Scene.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

Map::Map()
: Entity("map")
{
}

Map::~Map() {
   Map::tile_grid_t::iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      delete it->second;
   }
}

void Map::update(Scene& scene, Viewport& viewport) {
   Map::tile_grid_t::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      it->second->update(scene, viewport);
   }
}

void Map::add(int x, int y, Entity* tile) {
   this->add(sf::Vector2i(x, y), tile);
}

void Map::add(sf::Vector2f pos, Entity* tile) {
   this->add(static_cast<sf::Vector2i>(pos), tile);
}

void Map::add(sf::Vector2i pos, Entity* tile) {
   if (this->tiles_[pos]) {
      delete this->tiles_[pos];
   }

   this->tiles_[pos] = tile;
}

Map::tiles_t Map::intersects(sf::Vector2i point) {
   Map::tiles_t tiles;
   Map::tile_grid_t::const_iterator it;

   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>(it->second->get("physics"));
      if (tile_physics && tile_physics->intersects(point)) {
         tiles.push_back(it->second);
      }
   }

   return tiles;
}

Map::tiles_t Map::intersects(sf::Vector2f point) {
   return this->intersects(static_cast<sf::Vector2i>(point));
}

Map::tiles_t Map::intersects(sf::FloatRect rect) {
   Map::tiles_t tiles;
   Map::tile_grid_t::const_iterator it;

   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>(it->second->get("physics"));
      if (tile_physics && tile_physics->intersects(rect)) {
         tiles.push_back(it->second);
      }
   }

   return tiles;
}

std::string Map::to_string() {
   return "[Map]";
}
