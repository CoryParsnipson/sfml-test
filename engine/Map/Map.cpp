#include <cassert>

#include "Map.h"
#include "RenderSurface.h"
#include "Scene.h"
#include "Entity.h"
#include "PhysicsPart.h"
#include "Grid.h"
#include "OrthographicGrid.h"
#include "Serializer.h"

Map::Map(std::string id /* = "map" */)
: id_(id)
, grid_(new OrthographicGrid("default grid", 10))
{
}

Map::~Map() {
   delete this->grid_;
   this->grid_ = nullptr;
}

void Map::id(std::string id) {
   this->id_ = id;
}

std::string Map::to_string() {
   return "[Map \"" + this->id_ + "\"]";
}

std::string Map::serialize(Serializer& s) {
   Serializer::SerialData data;

   // serialize meta data
   data["id"] = this->id_;
   data["type"] = "map";

   // serialize grid
   data["grid"] = this->grid_->serialize(s);

   // serialize tiles
   Serializer::SerialData tile_data;
   
   Map::iterator it;
   for (it = this->begin(); it != this->end(); ++it) {
      tile_data[it->id()] = (*it)->serialize(s);
   }
   data["tiles"] = s.serialize(tile_data);

   return s.serialize(data);
}

void Map::deserialize(Serializer& s, Game& g, std::string& d) {
   Serializer::SerialData map_data = s.deserialize(g, d);

   assert(map_data["type"] == "map");

   // deserialize map meta data
   this->id(map_data["id"]);

   // deserialize grid
   Serializer::SerialData grid_data = s.deserialize(g, map_data["grid"]);

   if (grid_data["class"] == "OrthographicGrid") {
      Grid* grid = new OrthographicGrid(grid_data["id"]);
      grid->deserialize(s, g, map_data["grid"]);
      this->add(grid);
   } else {
      assert(("Received invalid grid class '" + grid_data["class"] + "'") == "");
   }

   // deserialize child entities (tiles, etc)
   Serializer::SerialData tile_data = s.deserialize(g, map_data["tiles"]);
   for (Serializer::SerialData::iterator it = tile_data.begin(); it != tile_data.end(); ++it) {
      Entity* tile = new Entity(it->first);
      tile->deserialize(s, g, it->second);
      this->add(tile);
   }
}

Grid* Map::grid() {
   return this->grid_;
}

void Map::add(Grid* grid) {
   delete this->grid_;
   this->grid_ = grid;
}

bool Map::intersects(const sf::Vector2i& other) { return true; }
bool Map::intersects(const sf::Vector2f& other) { return true; }
bool Map::intersects(const sf::FloatRect& other) { return true; }
bool Map::intersects(const SceneObject& other) { return true; }

void Map::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this);
}
