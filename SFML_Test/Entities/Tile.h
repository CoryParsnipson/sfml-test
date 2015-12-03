#ifndef TILE_H
#define TILE_H

#include "Entity.h"

class Tile : public Entity {
public:
   Tile(GraphicsComponent* graphics);
   virtual ~Tile();

   void set_position(sf::Vector2i pos);
   sf::Vector2i get_position();

   bool contains(sf::Vector2i coord);

   std::string to_string();

   virtual void update(Viewport& viewport);

protected:
   sf::Vector2i map_coordinate;
};

#endif
