#ifndef TILE_FACTORY_H
#define TILE_FACTORY_H

#include "Entity2.h"
#include "Texture.h"

class TileFactory {
public:
   static TileFactory* inst();
   virtual ~TileFactory();

   virtual Entity2* create_tile(Texture& t);
   virtual Entity2* create_tile(Texture& t, sf::Vector2f pos);

protected:
   // seal off constructors
   TileFactory();
   TileFactory(TileFactory& f);

   static TileFactory* inst_;
};

#endif
