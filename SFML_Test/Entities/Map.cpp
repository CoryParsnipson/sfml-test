#include "Map.h"

#include "../Coordinate/MapCoordinate.h"
#include "../FileReader/MapFileReader.h"

Map::Map() {
}

Map::~Map() {
	// delete all sprite pointers in the tilemap
	std::map<MapCoordinate, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		delete iterator->second;
	}
}

void Map::load_mapfile(std::string map_filename) {
	std::string mapfile_line;
	MapFileReader mfr(map_filename);

	std::vector<int>* tokens = new std::vector<int>();
	while (mfr.readline(&mapfile_line)) {
		mfr.tokenize(tokens, mapfile_line);

		if (!tokens->size()) {
			continue;
		}

		// generate map coordinates from mapfile line
		MapCoordinate map_coord((*tokens)[0], (*tokens)[1]);

		sf::Sprite* sprite = new sf::Sprite();
		sprite->setTexture(*(this->textures_tiles[(*tokens)[2]]));

		ViewportCoordinate viewport_coord = map_coord.to_screen_coordinates();
		sprite->setPosition((float)viewport_coord.getX(), (float)viewport_coord.getY());
		sprite->setScale(sf::Vector2f((float)Settings::Instance()->SCALE_X, (float)Settings::Instance()->SCALE_Y));

		this->tiles[map_coord] = sprite;

		tokens->clear();
	}

	delete tokens;
}

// returns an integer that corresponds to the index of the newly registered texture (to refer to it later)
// will return -1 on failure
int Map::register_texture(sf::Texture* texture) {
	this->textures_tiles.push_back(texture);
	return this->textures_tiles.size() - 1;
}

void Map::draw(sf::RenderWindow& window) {
	std::map<MapCoordinate, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		window.draw(*iterator->second);
	}
}

std::string Map::to_string() {
	std::string output;

	std::map<MapCoordinate, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		output += iterator->first.to_string() + "\n";
	}

	return output;
}