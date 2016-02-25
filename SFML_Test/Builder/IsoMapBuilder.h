#ifndef ISO_MAP_BUILDER_H
#define ISO_MAP_BUILDER_H

#include "MapBuilder.h"

class IsoMapBuilder : public MapBuilder {
public:
   IsoMapBuilder(TextureManager& tm);
   virtual ~IsoMapBuilder();

   virtual void build();

protected:
   virtual void build_map();
   virtual void build_tile(int x, int y, std::string texture);
   virtual void build_grid(const std::string& id, int tile_size);
   virtual void build_grid(const std::string& id, const sf::Vector2f& tile_size);
   virtual void build_grid(const std::string& id, const sf::Vector2f& tile_size, const sf::Vector2f& origin);
};

#endif
