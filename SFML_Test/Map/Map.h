#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"
#include "Entity.h"

// needed for comparison function in std::map with Vector2i as key
struct vector2i_cmp {
   bool operator()(const sf::Vector2i& l, const sf::Vector2i& r) const {
      return (l.x > r.x || (l.x == r.x && l.y > r.y));
   }
};

class Map
: public Draw
, public Update
{
public:
   typedef std::vector<Entity*> TileList;
   typedef std::map<sf::Vector2i, Entity*, vector2i_cmp> TileGrid;

   Map();
   virtual ~Map();

   // draw interface 
   virtual void draw(Graphics& graphics);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   virtual void add(int x, int y, Entity* tile);
   virtual void add(sf::Vector2f pos, Entity* tile);
   virtual void add(sf::Vector2i pos, Entity* tile); // map assumes ownership of supplied entity (change this in the future when using smart pointers)

   virtual TileList intersects(sf::Vector2i point);
   virtual TileList intersects(sf::Vector2f point);
   virtual TileList intersects(sf::FloatRect rect);

   virtual TileGrid get_tiles();
   virtual std::string to_string();
 
protected:
   TileGrid tiles_;
};

#endif
