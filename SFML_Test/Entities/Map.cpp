#include "Map.h"

#include "../FileReader/MapFileReader.h"

Map::Map() {
}

Map::~Map() {
	// delete all sprite pointers in the tilemap
	std::map<sf::Vector2i, Tile*>::const_iterator iterator;
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
		sf::Vector2i map_coord((*tokens)[0], (*tokens)[1]);
		this->tiles[map_coord] = new Tile(map_coord, *(this->textures_tiles[(*tokens)[2]]));
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
	std::map<sf::Vector2i, Tile*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		iterator->second->draw(window);
	}
}

std::string Map::to_string() {
	std::string output;

	std::map<sf::Vector2i, Tile*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		output += "Map (" + std::to_string(iterator->first.x) + ", " + std::to_string(iterator->first.y) + ")\n";
	}

	return output;
}