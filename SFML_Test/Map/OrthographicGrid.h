#ifndef ORTHOGRAPHIC_GRID_H
#define ORTHOGRAPHIC_GRID_H

#include "Grid.h"

class Graphic;

class OrthographicGrid
: public Grid
{
public:
   typedef std::list<Graphic*> GridlineList;

   OrthographicGrid(const std::string& id);
   OrthographicGrid(const std::string& id, int tile_size);
   OrthographicGrid(const std::string& id, const sf::Vector2f& tile_size);
   virtual ~OrthographicGrid();

   virtual void origin(const sf::Vector2f& origin);
   virtual const sf::Vector2f& origin();

   virtual sf::Vector2f coord_to_screen(const sf::Vector2f& coord);

   virtual void move(const sf::Vector2f& delta);
   virtual void set_scale(float factor);
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f floor(const sf::Vector2f& pos);
   virtual sf::Vector2f round(const sf::Vector2f& pos);
   virtual sf::Vector2f ceil(const sf::Vector2f& pos);

   // draw interface 
   virtual void draw(Graphics& graphics);
   virtual void layer(Layer* layer);
   virtual Layer* layer();

protected:
   Graphic* origin_dot_;
   GridlineList grid_cols_;
   GridlineList grid_rows_;
   
   void create_origin_dot();
   void create_gridlines();
   void clear_gridlines();
};

#endif
