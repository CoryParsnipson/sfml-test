#ifndef FLAT_MAP_H
#define FLAT_MAP_H

#include "Map.h"

class FlatMap : public Map {
public:
   typedef std::map<sf::Vector2i, Entity*, vector2i_cmp> map_type_t;
   
   FlatMap();
   virtual ~FlatMap();

   virtual void update(Game& game, Viewport& viewport);

   virtual Entity* set_tile(sf::Vector2i pos, Entity* tile);

   virtual sf::Vector2f get_map_to_screen_pos(sf::Vector2i tile_pos);
   virtual sf::Vector2i get_screen_to_map_pos(sf::Vector2f viewport_pos);

   virtual std::string to_string();

protected:
   map_type_t grid_;
};

#endif
