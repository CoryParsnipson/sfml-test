#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H

#include "dependencies.h"

// forward declarations
class Entity;
class TextureManager;
class Map;

class MapBuilder {
public:
   MapBuilder(TextureManager& tm) : texture_manager_(tm) {};
   virtual ~MapBuilder() {};

   virtual Map* get_map() { return this->map_; }

   virtual void create_map() = 0;
   virtual Entity* load_tile() = 0;
   //virtual saveTile() = 0;

protected:
   std::string id;
   TextureManager& texture_manager_;

   Map* map_;
};

#endif
