#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

// forward declarations
class Game;
class Entity;
class Viewport;

class Map {
public:
   typedef std::vector<Entity*> tiles_t;

   Map();
   virtual ~Map();

   // public interface
   virtual void update(Game& game, Viewport& viewport);

   virtual void add(Entity* tile); // map assumes ownership of supplied entity

   //virtual sf::Vector2f get_map_to_screen_pos(int x, int y) {
   //   return this->get_map_to_screen_pos(sf::Vector2i(x, y));
   //};
   //virtual sf::Vector2f get_map_to_screen_pos(sf::Vector2i pos) = 0;

   virtual std::string to_string();
 
protected:
   tiles_t tiles_;
};

#endif
