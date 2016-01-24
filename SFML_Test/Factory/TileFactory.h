#ifndef TILE_FACTORY_H
#define TILE_FACTORY_H

#include "Entity.h"
#include "Texture.h"

class TileFactory {
public:
   static TileFactory* inst();
   virtual ~TileFactory();

   virtual Entity* create_tile(Texture& t);
   virtual Entity* create_tile(Texture& t, sf::Vector2f pos);

   virtual Entity* create_tile_cursor(sf::Vector2f pos, sf::Vector2f size, std::vector<Entity*> references);

protected:
   // seal off constructors
   TileFactory();
   TileFactory(TileFactory& f);

   static TileFactory* inst_;
};

#endif
