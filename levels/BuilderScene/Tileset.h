#ifndef TILESET_H
#define TILESET_H

#include <string>

#include "Serializer.h"
#include "Serializable.h"

// ----------------------------------------------------------------------------
// Tileset
// 
// Holds information for different tile sets.
// ----------------------------------------------------------------------------
class Tileset {
public:
   using TileData = std::map<std::string, Serializer::SerialData>;

   Tileset();
   ~Tileset();

   std::string id() const;
   std::string texture_filename() const;

   Serializer::SerialData get_tile(unsigned int idx);
   Serializer::SerialData get_tile(std::string tile_id);

   void add_tile(std::string tile_id, Serializer::SerialData tile_data);

   unsigned int num_tiles() const;

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   std::string id_; // this is also the texture atlas key
   std::string texture_filename_;

   TileData tiledata_;
};

#endif
