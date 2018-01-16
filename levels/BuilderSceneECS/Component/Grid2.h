#ifndef GRID2_H
#define GRID2_H

#include "sfml.h"

#include "Component.h"

// ----------------------------------------------------------------------------
// Grid2
// 
// This marks an entity as a grid_root.
// ----------------------------------------------------------------------------
class Grid2 : public Component {
public:
   explicit Grid2(const std::string& id = "Grid2 Component", sf::Vector2f origin = sf::Vector2f(0, 0), unsigned int width = 1, unsigned int height = 1);
   Grid2(const Grid2& other);
   virtual ~Grid2();

   Grid2& operator=(const Grid2& other);
   void swap(Grid2& other);

   unsigned int tile_width() const;
   void tile_width(unsigned int width);

   unsigned int tile_height() const;
   void tile_height(unsigned int height);

   sf::Vector2f floor(const sf::Vector2f& pos);
   sf::Vector2f round(const sf::Vector2f& pos);
   sf::Vector2f ceil(const sf::Vector2f& pos);

private:
   sf::Vector2f origin_;

   unsigned int tile_width_;
   unsigned int tile_height_;
};

#endif
