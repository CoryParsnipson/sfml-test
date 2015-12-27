#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

#include "MapBuilder.h"

// forward declarations
class Game;
class Entity;
class Viewport;

class Map {
public:
   Map() {};
   virtual ~Map() {};

   // public interface
   virtual void update(Game& game, Viewport& viewport) = 0; 

   virtual Entity* set_tile(sf::Vector2i pos, Entity* tile) = 0;
   virtual Entity* set_tile(int x, int y, Entity* tile) = 0;

   virtual sf::Vector2f get_map_to_screen_pos(sf::Vector2i pos) = 0;
   virtual sf::Vector2f get_map_to_screen_pos(int x, int y) = 0;

   virtual std::string to_string() { return "[Map]"; };
 
protected:
};

#endif
