#include "Map.h"

#include "Scene.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"

#include "Grid.h"
#include "OrthographicGrid.h"

Map::Map()
: grid_(new OrthographicGrid("default grid", 10))
{
}

Map::~Map() {
   Map::TileList::iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      delete *it;
   }

   delete this->grid_;
   this->grid_ = nullptr;
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

void Map::add(Grid* grid) {
   delete this->grid_;
   this->grid_ = grid;
}

void Map::remove(Entity* tile) {
   Map::TileList::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      if (tile == (*it)) {
         this->tiles_.erase(it);
         delete tile;
         tile = nullptr;
         return;
      }
   }
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

Grid* Map::grid() {
   return this->grid_;
}
