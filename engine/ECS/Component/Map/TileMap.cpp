#include <cassert>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>

#include "TileMap.h"

TileMap::TileMap(const std::string& id /* = "TileMap Component" */)
: Component(id)
{
}

TileMap::TileMap(const TileMap& other)
: Component(other.id())
, tiles_(other.tiles_)
{
}

TileMap::~TileMap() {
   this->tiles_.clear();
}

TileMap& TileMap::operator=(const TileMap& other) {
   TileMap tmp(other);
   this->swap(tmp);
   return *this;
}

void TileMap::swap(TileMap& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   this->tiles_.swap(other.tiles_);
}

TileMap::TileType* TileMap::get(const sf::Vector2f& pos) {
   sf::Vector2f tile_pos;

   for (std::vector<TileMap::TileType>::iterator it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      if (this->get_position_from_tile(*it) == pos) {
         return &(*it);
      }
   }

   return nullptr;
}

void TileMap::set(TileMap::TileType tile) {
   this->tiles_.push_back(tile);
}

std::vector<TileMap::TileType*> TileMap::find(const sf::FloatRect& search_area) {
   std::vector<TileMap::TileType*> matching_tiles;

   std::for_each(this->tiles_.begin(), this->tiles_.end(),
      [&] (TileMap::TileType& tile) {
         sf::FloatRect gbounds = this->get_global_bounds_from_tile(tile);
         if (search_area.intersects(gbounds)) {
            matching_tiles.push_back(&tile);
         }
      }
   );

   return matching_tiles;
}

sf::Vector2f TileMap::get_position_from_tile(TileType& tile) {
   if (tile.type() == typeid(Circle)) {
      return boost::get<Circle>(tile).position();
   } else if (tile.type() == typeid(Rectangle)) {
      return boost::get<Rectangle>(tile).position();
   } else if (tile.type() == typeid(Sprite)) {
      return boost::get<Sprite>(tile).position();
   } else if (tile.type() == typeid(Text)) {
      return boost::get<Text>(tile).position();
   } else if (tile.type() == typeid(VertexList)) {
      return boost::get<VertexList>(tile).position();
   }

   throw std::domain_error("Received invalid tile type: " + std::string(tile.type().name()));
}

sf::FloatRect TileMap::get_global_bounds_from_tile(TileType& tile) {
   if (tile.type() == typeid(Circle)) {
      return boost::get<Circle>(tile).global_bounds();
   } else if (tile.type() == typeid(Rectangle)) {
      return boost::get<Rectangle>(tile).global_bounds();
   } else if (tile.type() == typeid(Sprite)) {
      return boost::get<Sprite>(tile).global_bounds();
   } else if (tile.type() == typeid(Text)) {
      return boost::get<Text>(tile).global_bounds();
   } else if (tile.type() == typeid(VertexList)) {
      return boost::get<VertexList>(tile).global_bounds();
   }

   throw std::domain_error("Received invalid tile type: " + std::string(tile.type().name()));

   return sf::FloatRect(0, 0, 0, 0);
}
