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

   virtual void add(Entity* tile); // map assumes ownership of supplied entity (change this in the future when using smart pointers)

   virtual tiles_t intersects(sf::Vector2i point);
   virtual tiles_t intersects(sf::Vector2f point);
   virtual tiles_t intersects(sf::FloatRect rect);

   virtual std::string to_string();
 
protected:
   tiles_t tiles_;
};

#endif
