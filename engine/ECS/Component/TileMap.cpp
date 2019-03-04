#include <cassert>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>

#include "TileMap.h"

TileMap::TileMap(const std::string& id /* = "TileMap Component" */)
: Component(id)
, tiles_()
, z_index_(0)
{
}

TileMap::TileMap(const TileMap& other)
: Component(other.id())
, tiles_(other.tiles_)
, z_index_(other.z_index_)
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
   std::swap(this->z_index_, other.z_index_);
}

int TileMap::z_index() const {
   return this->z_index_;
}

void TileMap::z_index(int z_index) {
   this->z_index_ = z_index;
}

TileMap::TileType* TileMap::get(const sf::Vector2f& pos) {
   TileMap::TileStore::iterator it = this->tiles_.find(pos);

   if (it != this->tiles_.end()) {
      return &(it->second);
   }

   return nullptr;
}

std::vector<TileMap::TileType*> TileMap::find(const sf::FloatRect& search_area) {
   std::vector<TileMap::TileType*> matching_tiles;
   for (TileMap::TileStore::iterator it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      sf::FloatRect gbounds = this->get_global_bounds_from_tile(it->second);
      if (search_area.intersects(gbounds)) {
         matching_tiles.push_back(&it->second);
      }
   }

   return matching_tiles;
}

void TileMap::set(TileMap::TileType tile) {
   this->tiles_[this->get_position_from_tile(tile)] = tile;
}

void TileMap::remove(TileMap::TileType tile) {
   bool is_match = false;
   sf::Vector2f tile_pos = this->get_position_from_tile(tile);
   TileMap::TileType* t = this->get(tile_pos);

   if (tile.type() == typeid(Circle)) {
      is_match = boost::get<Circle>(tile).id() == boost::get<Circle>(*t).id();
   } else if (tile.type() == typeid(Rectangle)) {
      is_match = boost::get<Rectangle>(tile).id() == boost::get<Rectangle>(*t).id();
   } else if (tile.type() == typeid(Sprite)) {
      is_match = boost::get<Sprite>(tile).id() == boost::get<Sprite>(*t).id();
   } else if (tile.type() == typeid(Text)) {
      is_match = boost::get<Text>(tile).id() == boost::get<Text>(*t).id();
   } else if (tile.type() == typeid(VertexList)) {
      is_match = boost::get<VertexList>(tile).id() == boost::get<VertexList>(*t).id();
   }

   if (is_match) {
      this->tiles_.erase(tile_pos);
   }
}

void TileMap::remove(const sf::FloatRect& search_area) {
   std::vector<TileMap::TileType*> tiles = this->find(search_area);

   for (auto tile : tiles) {
      this->tiles_.erase(this->get_position_from_tile(*tile));
   }
}

std::string TileMap::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["type"] = "TileMap";
   data["id"] = this->id();
   
   data["num_tiles"] = std::to_string(this->tiles_.size());
   data["z_index"] = std::to_string(this->z_index());

   for (TileMap::TileStore::iterator it = this->tiles_.begin(); it != this->tiles_.end(); ++it) {
      std::size_t tile_hash = TileMap::TileHash()(it->first);

      if (it->second.type() == typeid(Circle)) {
         data["tile_" + std::to_string(tile_hash)] = boost::get<Circle>(it->second).serialize(s);
      } else if (it->second.type() == typeid(Rectangle)) {
         data["tile_" + std::to_string(tile_hash)] = boost::get<Rectangle>(it->second).serialize(s);
      } else if (it->second.type() == typeid(Sprite)) {
         data["tile_" + std::to_string(tile_hash)] = boost::get<Sprite>(it->second).serialize(s);
      } else if (it->second.type() == typeid(Text)) {
         data["tile_" + std::to_string(tile_hash)] = boost::get<Text>(it->second).serialize(s);
      } else if (it->second.type() == typeid(VertexList)) {
         data["tile_" + std::to_string(tile_hash)] = boost::get<VertexList>(it->second).serialize(s);
      } else {
         throw std::domain_error("Received invalid tile type: " + std::string(it->second.type().name()));
      }
   }
   
   return s.serialize(data);
}

void TileMap::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->tiles_.clear();
   this->id(data["id"]);
   this->z_index(std::stoi(data["z_index"]));

   for (auto kv_pair : data) {
      if (kv_pair.first.rfind("tile_", 0) != 0) {
         continue;
      }

      Serializer::SerialData tile_data = s.deserialize(scene, kv_pair.second);

      if (tile_data["type"] == "Circle") {
         Circle c;
         c.deserialize(s, scene, kv_pair.second);
         this->set(c);
      } else if (tile_data["type"] == "Rectangle") {
         Rectangle r;
         r.deserialize(s, scene, kv_pair.second);
         this->set(r);
      } else if (tile_data["type"] == "Sprite") {
         Sprite sprite;
         sprite.deserialize(s, scene, kv_pair.second);
         this->set(sprite);
      } else if (tile_data["type"] == "Text") {
         Text t;
         t.deserialize(s, scene, kv_pair.second);
         this->set(t);
      } else if (tile_data["type"] == "VertexList") {
         VertexList v;
         v.deserialize(s, scene, kv_pair.second);
         this->set(v);
      } else {
         throw std::domain_error("Received invalid tile type: " + tile_data["type"]);
      }
   }
}

sf::Vector2f TileMap::get_position_from_tile(TileType& tile) {
   if (tile.type() == typeid(Circle)) {
      return boost::get<Circle>(tile).offset();
   } else if (tile.type() == typeid(Rectangle)) {
      return boost::get<Rectangle>(tile).offset();
   } else if (tile.type() == typeid(Sprite)) {
      return boost::get<Sprite>(tile).offset();
   } else if (tile.type() == typeid(Text)) {
      return boost::get<Text>(tile).offset();
   } else if (tile.type() == typeid(VertexList)) {
      return boost::get<VertexList>(tile).offset();
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
