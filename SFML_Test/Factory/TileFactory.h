#ifndef TILE_FACTORY_H
#define TILE_FACTORY_H

#include "Entity.h"
#include "Texture.h"

class TileFactory {
public:
   static TileFactory* inst();
   virtual ~TileFactory();

   virtual Entity* create_tile(Texture& t, Layer* layer = nullptr);
   virtual Entity* create_tile(Texture& t, sf::Vector2f pos, Layer* layer = nullptr);

   virtual Entity* create_tile_cursor(sf::Vector2f& one, sf::Vector2f& two, std::vector<Entity*> references, Layer* layer = nullptr);

   virtual Entity* create_selection_rectangle(sf::FloatRect* bounds = nullptr, Layer* layer = nullptr);

protected:
   // seal off constructors
   TileFactory();
   TileFactory(TileFactory& f);

   static TileFactory* inst_;
};

#endif
