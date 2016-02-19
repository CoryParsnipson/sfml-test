#include "Map.h"

#include "Scene.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

Map::Map()
{
}

Map::~Map() {
   Map::TileList::iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      delete *it;
   }
}

void Map::draw(Graphics& graphics) {
   Map::TileList::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      (*it)->draw(graphics);
   }
}

void Map::layer(Layer* layer) {
   Map::TileList::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      (*it)->layer(layer);
   }
}

void Map::update(Game& game, Scene* scene, Entity* entity) {
   Map::TileList::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      (*it)->update(game, scene);
   }
}

void Map::add(Entity* tile) {
   // check to see if entity is already in map and abort
   Map::TileList::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      if (tile == (*it)) {
         Service::get_logger().msg("Map", Logger::WARNING, "Ignoring request to add tile which is already in map.");
         return;
      }
   }

   this->tiles_.push_back(tile);
}

Map::TileList Map::intersects(sf::Vector2i point) {
   Map::TileList tiles;
   Map::TileList::const_iterator it;

   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>((*it)->get("physics"));
      if (tile_physics && tile_physics->intersects(point)) {
         tiles.push_back(*it);
      }
   }

   return tiles;
}

Map::TileList Map::intersects(sf::Vector2f point) {
   return this->intersects(static_cast<sf::Vector2i>(point));
}

Map::TileList Map::intersects(sf::FloatRect rect) {
   Map::TileList tiles;
   Map::TileList::const_iterator it;

   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>((*it)->get("physics"));
      if (tile_physics && tile_physics->intersects(rect)) {
         tiles.push_back(*it);
      }
   }

   return tiles;
}

const Map::TileList& Map::get_tiles() {
   return this->tiles_;
}

std::string Map::to_string() {
   return "[Map]";
}
