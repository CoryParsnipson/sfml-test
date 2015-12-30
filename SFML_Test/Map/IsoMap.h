#ifndef ISO_MAP_H
#define ISO_MAP_H

#include "Map.h"

class IsoMap : public Map {
public:
   typedef std::map<sf::Vector2i, Entity*, vector2i_cmp> map_type_t;

   IsoMap();
   virtual ~IsoMap();

   virtual void update(Game& game, Viewport& viewport);

   virtual Entity* set_tile(sf::Vector2i pos, Entity* tile);
   virtual sf::Vector2f get_map_to_screen_pos(sf::Vector2i pos);
   virtual std::string to_string();

protected:
   map_type_t grid_;
};

#endif
