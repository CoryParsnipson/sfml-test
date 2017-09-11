#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <vector>
#include <string>

#include "sfml.h"

#include "Component.h"
#include "Sprite.h"

// ----------------------------------------------------------------------------
// TileMap
//
// This component is for modular (tiled) backgrounds. This component is a
// mapping between locations and tile textures.
// ----------------------------------------------------------------------------
class TileMap : public Component {
public:
   using Tile = Sprite;

   explicit TileMap(const std::string& id = "TileMap Component");
   TileMap(const TileMap& other);
   virtual ~TileMap();

   TileMap& operator=(const TileMap& other);
   void swap(TileMap& other);

   Tile* get(const sf::Vector2f& pos);
   void set(Tile tile);

   std::vector<Tile*> find(const sf::FloatRect& search_area);

private:
   std::vector<Tile> tiles_;
};

#endif
