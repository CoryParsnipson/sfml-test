#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H

#include "dependencies.h"

// forward declarations
class Entity;
class Serializer;
class TextureManager;
class Map;

class MapBuilder {
public:
   MapBuilder(TextureManager& tm) : serializer_(nullptr), texture_manager_(tm) {};
   virtual ~MapBuilder() {};

   virtual void create_serializer(std::string filename) = 0;
   virtual void build() = 0; // call this to build the entire thing

   virtual Map* get_map() { return this->map_; }

protected:
   virtual void build_map() = 0;
   virtual void build_tile(int x, int y, int texture) = 0;

   std::string id;

   Serializer* serializer_;
   TextureManager& texture_manager_;

   Map* map_;
};

#endif
