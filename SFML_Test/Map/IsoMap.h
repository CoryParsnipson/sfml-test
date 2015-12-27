#ifndef ISO_MAP_H
#define ISO_MAP_H

#include "Map.h"

// needed for comparison function in std::map with Vector2i as key
struct vector2i_cmp {
	bool operator()(const sf::Vector2i& l, const sf::Vector2i& r) const {
		return (l.x < r.x || (l.x == r.x && l.y < r.y));
	}
};

class IsoMap : public Map {
public:
   typedef std::map<sf::Vector2i, Entity*, vector2i_cmp> map_type_t;

   IsoMap();
   virtual ~IsoMap();

   virtual void update(Game& game, Viewport& viewport);

   virtual Entity* set_tile(sf::Vector2i pos, Entity* tile);
   virtual Entity* set_tile(int x, int y, Entity* tile);

   virtual sf::Vector2f get_map_to_screen_pos(sf::Vector2i pos);
   virtual sf::Vector2f get_map_to_screen_pos(int x, int y);

   virtual std::string to_string();

protected:
   map_type_t grid_;
};

#endif
