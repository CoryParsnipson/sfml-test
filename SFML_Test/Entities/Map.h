#ifndef MAP_H
#define MAP_H

#include "../dependencies.h"

#include "Texture.h"
#include "Tile.h"

// forward declarations
class Game;
class Graphics;

// needed for comparison function in std::map with Vector2i as key
struct vector2i_cmp {
	bool operator()(const sf::Vector2i& l, const sf::Vector2i& r) const {
		return (l.x < r.x || (l.x == r.x && l.y < r.y));
	}
};

class Map {
public:
   typedef std::map<sf::Vector2i, Tile*, vector2i_cmp> map_type_t;

	Map();
	~Map();

	void load_mapfile(Game& game, std::string map_filename);

	void draw(Graphics& graphics, Viewport& viewport);
	std::string to_string();

protected:
	std::string map_filename;

	map_type_t tiles;
};

#endif
