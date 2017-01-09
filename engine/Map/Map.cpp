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
   delete this->grid_;
   this->grid_ = nullptr;
}

std::string Map::to_string() {
   return "[Map]";
}

std::string Map::serialize(Serializer& s) {
   Serializer::SerialData data;
   data["grid"] = this->grid_->serialize(s);
   
   int i = 0;
   Map::iterator it;
   for (it = this->begin(); it != this->end(); ++it) {
      data["tile_" + std::to_string(i)] = (*it)->serialize(s);
      ++i;
   }

   return s.serialize(data);
}

void deserialize(Serializer& s, Game& g, Channel& c) {
   // TODO: implement me
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
