#ifndef ORTHOGRAPHIC_GRID_H
#define ORTHOGRAPHIC_GRID_H

#include "Grid.h"

class Entity;

class OrthographicGrid
: public Grid
{
public:
   OrthographicGrid(const std::string& id);
   OrthographicGrid(const std::string& id, int tile_size);
   OrthographicGrid(const std::string& id, const sf::Vector2f& tile_size);
   virtual ~OrthographicGrid();

   virtual void origin(const sf::Vector2f& origin);

   virtual sf::Vector2f coord_to_screen(const sf::Vector2f& coord);

   virtual sf::Vector2f floor(const sf::Vector2f& pos);
   virtual sf::Vector2f round(const sf::Vector2f& pos);
   virtual sf::Vector2f ceil(const sf::Vector2f& pos);

   // draw interface 
   virtual void draw(Graphics& graphics);
   virtual void layer(Layer* layer);
   virtual Layer* layer();

protected:
   Entity* origin_dot_;

   void create_origin_dot();
};

#endif
