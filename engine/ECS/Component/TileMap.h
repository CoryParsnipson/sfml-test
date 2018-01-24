#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <vector>
#include <string>

#include <boost/variant.hpp>

#include "sfml.h"

#include "Component.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Text.h"
#include "VertexList.h"

// ----------------------------------------------------------------------------
// TileMap
//
// This component is for modular (tiled) backgrounds. This component is a
// mapping between locations and tile textures.
// ----------------------------------------------------------------------------
class TileMap : public Component {
public:
   using TileType = boost::variant<Circle, Rectangle, Sprite, Text, VertexList>;

   explicit TileMap(const std::string& id = "TileMap Component");
   TileMap(const TileMap& other);
   virtual ~TileMap();

   TileMap& operator=(const TileMap& other);
   void swap(TileMap& other);

   TileType* get(const sf::Vector2f& pos);
   std::vector<TileType*> find(const sf::FloatRect& search_area);

   void set(TileType tile);

   void remove(TileType tile);
   void remove(const sf::FloatRect& search_area);

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   std::vector<TileType> tiles_;

   sf::Vector2f get_position_from_tile(TileType& tile);
   sf::FloatRect get_global_bounds_from_tile(TileType& tile);
};

#endif
