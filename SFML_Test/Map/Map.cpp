#include "Map.h"

#include "Scene.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

Map::Map()
{
}

Map::~Map() {
   Map::TileGrid::iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      delete it->second;
   }
}

void Map::draw(Graphics& graphics) {
   Map::TileGrid::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      it->second->draw(graphics);
   }
}

void Map::update(Game& game, Scene* scene, Entity* entity) {
   Map::TileGrid::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      it->second->update(game, scene);
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
      Service::get_logger().msg("Map", Logger::INFO, "Deleting old tile at (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")");
      delete this->tiles_[pos];
   }

   Service::get_logger().msg("Map", Logger::INFO, "Adding tile at (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")");
   this->tiles_[pos] = tile;
}

Map::TileList Map::intersects(sf::Vector2i point) {
   Map::TileList tiles;
   Map::TileGrid::const_iterator it;

   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>(it->second->get("physics"));
      if (tile_physics && tile_physics->intersects(point)) {
         tiles.push_back(it->second);
      }
   }

   return tiles;
}

Map::TileList Map::intersects(sf::Vector2f point) {
   return this->intersects(static_cast<sf::Vector2i>(point));
}

Map::TileList Map::intersects(sf::FloatRect rect) {
   Map::TileList tiles;
   Map::TileGrid::const_iterator it;

   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      PhysicsPart* tile_physics = dynamic_cast<PhysicsPart*>(it->second->get("physics"));
      if (tile_physics && tile_physics->intersects(rect)) {
         tiles.push_back(it->second);
      }
   }

   return tiles;
}

Map::TileGrid Map::get_tiles() {
   return this->tiles_;
}

std::string Map::to_string() {
   return "[Map]";
}
