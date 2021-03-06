#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <boost/variant.hpp>
#include <SFML/Graphics.hpp>

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
   // -------------------------------------------------------------------------
   // This is provided to the unordered map to hash map tile graphics by
   // position.
   // -------------------------------------------------------------------------
   class TileHash {
   public:
      std::size_t operator()(const sf::Vector2f& k) const {
         // to combine hashes, multiple one by a prime constant first
         // (this generalizes to higher dimensions)
         return std::hash<float>()(k.x) ^ (std::hash<float>()(k.y) << 1);
      }
   };

   using TileType = boost::variant<Circle, Rectangle, Sprite, Text, VertexList>;
   using TileStore = std::unordered_map<sf::Vector2f, TileType, TileHash>;

   explicit TileMap(const std::string& id = "TileMap Component");
   TileMap(const TileMap& other);
   virtual ~TileMap();

   TileMap& operator=(const TileMap& other);
   void swap(TileMap& other);

   int z_index() const;
   void z_index(int z_index);

   TileType* get(const sf::Vector2f& pos);
   std::vector<TileType*> find(const sf::FloatRect& search_area);

   void set(TileType tile);

   void remove(TileType tile);
   void remove(const sf::FloatRect& search_area);

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   TileStore tiles_;
   int z_index_;

   sf::Vector2f get_position_from_tile(TileType& tile);
   sf::FloatRect get_global_bounds_from_tile(TileType& tile);
};

#endif
