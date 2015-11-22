#ifndef MAP_H
#define MAP_H

#include "../dependencies.h"

#include "Texture.h"
#include "Tile.h"

// needed for comparison function in std::map with Vector2i as key
struct vector2i_cmp {
	bool operator()(const sf::Vector2i& l, const sf::Vector2i& r) const {
		return (l.x < r.x || (l.x == r.x && l.y < r.y));
	}
};

class Map {
public:
	Map();
	~Map();

	int register_texture(Texture* texture);
	void load_mapfile(std::string map_filename);

	void draw(sf::RenderWindow& window);
	std::string to_string();

protected:
	std::string map_filename;

	std::vector<Texture*> textures_tiles;
	std::map<sf::Vector2i, Tile*, vector2i_cmp> tiles;
};

#endif
