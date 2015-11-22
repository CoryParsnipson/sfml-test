#ifndef TILE_H
#define TILE_H

#include "../dependencies.h"

//
// Tile class for unit section of map
//
// this is meant to contain sprites (and by association, screen position)
// and information about solid-ness, variant, orientation, etc
//
class Tile {
public:
	Tile(sf::Vector2i& pos, Texture& t);
	~Tile();

	void draw(sf::RenderWindow& window);
   
protected:
	sf::Vector2i& map_coordinate; // should be identical to map key

	sf::Sprite* sprite;
};

#endif
