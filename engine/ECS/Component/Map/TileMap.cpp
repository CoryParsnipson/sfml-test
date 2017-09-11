#include <cassert>
#include <algorithm>

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

TileMap::Tile* TileMap::get(const sf::Vector2f& pos) {
   for (std::vector<TileMap::Tile>::iterator it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      if ((*it).position() == pos) {
         return &(*it);
      }
   }

   return nullptr;
}

void TileMap::set(TileMap::Tile tile) {
   this->tiles_.push_back(tile);
}

std::vector<TileMap::Tile*> TileMap::find(const sf::FloatRect& search_area) {
   std::vector<TileMap::Tile*> matching_tiles;

   std::for_each(this->tiles_.begin(), this->tiles_.end(),
      [&] (TileMap::Tile& tile) {
         if (search_area.intersects(tile.global_bounds())) {
            matching_tiles.push_back(&tile);
         }
      }
   );

   return matching_tiles;
}
