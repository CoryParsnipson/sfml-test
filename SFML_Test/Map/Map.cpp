#include "Map.h"
#include "RenderSurface.h"
#include "Scene.h"
#include "Entity.h"
#include "PhysicsPart.h"
#include "Grid.h"
#include "OrthographicGrid.h"
#include "Serializer.h"

Map::Map()
: grid_(new OrthographicGrid("default grid", 10))
{
}

Map::~Map() {
   // TODO: come back to this!
   //Map::TileList::iterator it;
   //for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
   //   delete *it;
   //}

   delete this->grid_;
   this->grid_ = nullptr;
}

std::string Map::to_string() {
   return "[Map]";
}

void Map::serialize(Serializer& serializer) {
   // serialize the grid
   serializer.comment("grid");
   serializer.set(this->grid_->serialize());

   // serialize all tiles
   serializer.comment("map tiles");

   Map::TileList::const_iterator it;
   for (it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      serializer.set((*it)->serialize());
   }
}

Grid* Map::grid() {
   return this->grid_;
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

bool Map::intersects(const sf::Vector2i& other) { return false; }
bool Map::intersects(const sf::Vector2f& other) { return false; }
bool Map::intersects(const sf::FloatRect& other) { return false; }
bool Map::intersects(const SceneObject& other) { return false; }

void Map::accept(SceneGraphVisitor& visitor) {
   // TODO: implement me
}
