#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H

#include "dependencies.h"

#include "Map.h"
#include "TextureDictionary.h"

// forward declarations
class Entity;
class Serializer;

class MapBuilder {
public:
   MapBuilder() : serializer_(nullptr), grid_font_(nullptr), textures_(&this->default_textures_) {}
   virtual ~MapBuilder() {};

   virtual void build() = 0; // call this to build the entire thing

   virtual void set_serializer(Serializer* serializer) { this->serializer_ = serializer; };
   virtual Map* get_map() { return this->map_; }

   void grid_font(sf::Font* grid_font) {
      this->grid_font_ = grid_font;
   }

   void textures(const TextureDictionary* textures) {
      if (!textures) {
         this->textures_ = &this->default_textures_;
         return;
      }
      this->textures_ = textures;
   }

private:
   TextureDictionary default_textures_;

protected:
   virtual void build_map() = 0;
   virtual void build_tile(Entity* tile) = 0;
   virtual void build_grid(Grid* grid) = 0;

   std::string id;

   Serializer* serializer_;
   Map* map_;

   sf::Font* grid_font_;
   const TextureDictionary* textures_;
};

#endif
