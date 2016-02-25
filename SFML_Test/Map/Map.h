#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"
#include "Entity.h"

class Grid;

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
   virtual void layer(Layer* layer);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   void add(Entity* tile);
   void add(Grid* grid);

   virtual TileList intersects(sf::Vector2i point);
   virtual TileList intersects(sf::Vector2f point);
   virtual TileList intersects(sf::FloatRect rect);

   const TileList& get_tiles();
   virtual std::string to_string();

   Grid* grid();
 
protected:
   Grid* grid_;
   TileList tiles_;
};

#endif
