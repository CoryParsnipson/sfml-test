#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"
#include "Entity.h"

class Grid;
class Serializer;

class Map
: public Draw
, public Update
{
public:
   typedef std::vector<Entity*> TileList;

   Map();
   virtual ~Map();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   void add(Entity* tile);
   void add(Grid* grid);

   void remove(Entity* tile);

   virtual TileList intersects(sf::Vector2i point);
   virtual TileList intersects(sf::Vector2f point);
   virtual TileList intersects(sf::FloatRect rect);

   virtual std::string to_string();
   virtual void serialize(Serializer& serializer);

   Grid* grid();

protected:
   Grid* grid_;
   TileList tiles_;
};

#endif
