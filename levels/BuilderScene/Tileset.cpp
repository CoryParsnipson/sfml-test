#include <stdexcept>

#include "Tileset.h"

Tileset::Tileset() {
}

Tileset::~Tileset() {
}

std::string Tileset::id() const {
   return this->id_;
}

std::string Tileset::texture_filename() const {
   return this->texture_filename_;
}

Serializer::SerialData Tileset::get_tile(unsigned int idx) {
   if (idx >= this->num_tiles()) {
      throw std::out_of_range(
         "Cannot access tile #" + std::to_string(idx) + " of Tileset '" +
         this->id() + "' num tiles = " + std::to_string(this->num_tiles())
      );
   }

   TileData::iterator it = this->tiledata_.begin();
   for (unsigned int i = 0; i < idx; ++i) {
      ++it;
   }

   return it->second;
}

Serializer::SerialData Tileset::get_tile(std::string tile_id) {
   return this->tiledata_.at(tile_id);
}

void Tileset::add_tile(std::string tile_id, Serializer::SerialData tile_data) {
   this->tiledata_[tile_id] = tile_data;
}

unsigned int Tileset::num_tiles() const {
   return this->tiledata_.size();
}

std::string Tileset::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["id"] = this->id();
   data["filename"] = this->texture_filename();
   data["num_tiles"] = this->num_tiles();

   for (TileData::iterator it = this->tiledata_.begin(); it != this->tiledata_.end(); ++it) {
      data[it->first] = s.serialize(it->second);
   }

   return s.serialize(data);
}

void Tileset::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   this->id_ = data["id"];
   this->texture_filename_ = data["filename"];

   data.erase("id");
   data.erase("filename");
   data.erase("num_tiles");

   for (Serializer::SerialData::iterator it = data.begin(); it != data.end(); ++it) {
      this->add_tile(it->first, s.deserialize(scene, it->second));
   }
}
