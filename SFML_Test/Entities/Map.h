#ifndef MAP_H
#define MAP_H

#include "../dependencies.h"

#include "../Coordinate/MapCoordinate.h"

class Map {
public:
	Map();
	~Map();

	int register_texture(sf::Texture* texture);
	void load_mapfile(std::string map_filename);

	void draw(sf::RenderWindow& window);
	std::string to_string();

protected:
	std::string map_filename;

	std::vector<sf::Texture*> textures_tiles;
	std::map<MapCoordinate, sf::Sprite*> tiles;
};

#endif