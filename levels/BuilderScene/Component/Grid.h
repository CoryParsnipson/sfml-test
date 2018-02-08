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
   explicit Grid(const std::string& id = "Grid Component", sf::Vector2f origin = sf::Vector2f(0, 0), unsigned int width = 1, unsigned int height = 1);
   Grid(const Grid& other);
   virtual ~Grid();

   Grid& operator=(const Grid& other);
   void swap(Grid& other);

   unsigned int tile_width() const;
   void tile_width(unsigned int width);

   unsigned int tile_height() const;
   void tile_height(unsigned int height);

   sf::Vector2f floor(const sf::Vector2f& pos);
   sf::Vector2f round(const sf::Vector2f& pos);
   sf::Vector2f ceil(const sf::Vector2f& pos);

   sf::Vector2f zoom_factor;

private:
   sf::Vector2f origin_;

   unsigned int tile_width_;
   unsigned int tile_height_;
};

#endif
