#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>

#include "Component.h"

// ----------------------------------------------------------------------------
// Grid
// 
// This marks an entity as a grid_root.
// ----------------------------------------------------------------------------
class Grid : public Component {
public:
   explicit Grid(const std::string& id = "Grid Component", sf::Vector2f origin = sf::Vector2f(0, 0), float width = 32.f, float height = 32.f);
   Grid(const Grid& other);
   virtual ~Grid();

   Grid& operator=(const Grid& other);
   void swap(Grid& other);

   void origin(sf::Vector2f origin);
   void origin(float origin_x, float origin_y);
   const sf::Vector2f& origin() const;

   float tile_width() const;
   void tile_width(float width);

   float tile_height() const;
   void tile_height(float height);

   void zoom_factor(sf::Vector2f zoom_factor);
   void zoom_factor(float zoom_factor_x, float zoom_factor_y);
   const sf::Vector2f& zoom_factor() const;

   sf::Vector2f floor(const sf::Vector2f& pos);
   sf::Vector2f round(const sf::Vector2f& pos);
   sf::Vector2f ceil(const sf::Vector2f& pos);

   // this function rounds to the nearest grid point and returns a vector divided by tile height and width
   sf::Vector2i grid_index(const sf::Vector2f& pos);
   sf::Vector2i grid_index(float x, float y);

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   sf::Vector2f origin_;

   float tile_width_;
   float tile_height_;

   sf::Vector2f zoom_factor_;
};

#endif
