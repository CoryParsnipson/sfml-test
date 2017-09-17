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
}

std::string TileMap::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["type"] = "TileMap";
   data["id"] = this->id();
   
   data["num_tiles"] = std::to_string(this->tiles_.size());

   for (unsigned int i = 0; i < this->tiles_.size(); ++i) {
      TileType& tile(this->tiles_[i]);

      if (tile.type() == typeid(Circle)) {
         data["tile_" + std::to_string(i)] = boost::get<Circle>(tile).serialize(s);
      } else if (tile.type() == typeid(Rectangle)) {
         data["tile_" + std::to_string(i)] = boost::get<Rectangle>(tile).serialize(s);
      } else if (tile.type() == typeid(Sprite)) {
         data["tile_" + std::to_string(i)] = boost::get<Sprite>(tile).serialize(s);
      } else if (tile.type() == typeid(Text)) {
         data["tile_" + std::to_string(i)] = boost::get<Text>(tile).serialize(s);
      } else if (tile.type() == typeid(VertexList)) {
         data["tile_" + std::to_string(i)] = boost::get<VertexList>(tile).serialize(s);
      } else {
         throw std::domain_error("Received invalid tile type: " + std::string(tile.type().name()));
      }
   }

   return s.serialize(data);
}

void TileMap::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->tiles_.clear();
   this->id(data["id"]);

   for (int i = 0; i < std::stoi(data["num_tiles"]); ++i) {
      Serializer::SerialData tile_data = s.deserialize(scene, data["tile_" + std::to_string(i)]);

      if (tile_data["type"] == "Circle") {
         Circle c;
         c.deserialize(s, scene, data["tile_" + std::to_string(i)]);
         this->set(c);
      } else if (tile_data["type"] == "Rectangle") {
         Rectangle r;
         r.deserialize(s, scene, data["tile_" + std::to_string(i)]);
         this->set(r);
      } else if (tile_data["type"] == "Sprite") {
         Sprite sprite;
         sprite.deserialize(s, scene, data["tile_" + std::to_string(i)]);
         this->set(sprite);
      } else if (tile_data["type"] == "Text") {
         Text t;
         t.deserialize(s, scene, data["tile_" + std::to_string(i)]);
         this->set(t);
      } else if (tile_data["type"] == "VertexList") {
         VertexList v;
         v.deserialize(s, scene, data["tile_" + std::to_string(i)]);
         this->set(v);
      } else {
         throw std::domain_error("Received invalid tile type: " + tile_data["type"]);
      }
   }
}
