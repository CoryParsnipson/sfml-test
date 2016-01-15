#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

#include "Entity.h"

// forward declarations
class Scene;
class Viewport;

class Map
: public Entity
{
public:
   typedef std::vector<Entity*> tiles_t;

   Map();
   virtual ~Map();

   // public interface
   virtual void update(Scene& scene, Viewport& viewport);

   virtual void add(Entity* tile); // map assumes ownership of supplied entity (change this in the future when using smart pointers)

   virtual tiles_t intersects(sf::Vector2i point);
   virtual tiles_t intersects(sf::Vector2f point);
   virtual tiles_t intersects(sf::FloatRect rect);

   virtual std::string to_string();
 
protected:
   tiles_t tiles_;
};

#endif
