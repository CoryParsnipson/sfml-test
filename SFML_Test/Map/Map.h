#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"
#include "Entity.h"

class Map
: public Draw
, public Update
{
public:
   typedef std::vector<Entity*> TileList;

   Map();
   virtual ~Map();

   // draw interface 
   virtual void draw(Graphics& graphics);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   virtual void add(Entity* tile);

   virtual TileList intersects(sf::Vector2i point);
   virtual TileList intersects(sf::Vector2f point);
   virtual TileList intersects(sf::FloatRect rect);

   virtual const TileList& get_tiles();
   virtual std::string to_string();
 
protected:
   TileList tiles_;
};

#endif
