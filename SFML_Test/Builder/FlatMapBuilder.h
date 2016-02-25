#ifndef FLAT_MAP_BUILDER_H
#define FLAT_MAP_BUILDER_H

#include "MapBuilder.h"

class FlatMapBuilder : public MapBuilder {
public:
   FlatMapBuilder(TextureManager& tm);
   virtual ~FlatMapBuilder();

   virtual void build();

protected:
   virtual void build_map();
   virtual void build_tile(int x, int y, std::string texture);
   virtual void build_grid(const std::string& id, int tile_size);
   virtual void build_grid(const std::string& id, const sf::Vector2f& tile_size);
   virtual void build_grid(const std::string& id, const sf::Vector2f& tile_size, const sf::Vector2f& origin);
};

#endif
