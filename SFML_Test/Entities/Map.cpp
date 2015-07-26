#include "Map.h"

#include "../FileReader/MapFileReader.h"

Map::Map() {
}

Map::~Map() {
	// delete all sprite pointers in the tilemap
	std::map<sf::Vector2i, sf::Sprite*>::const_iterator iterator;
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

		sf::Sprite* sprite = new sf::Sprite();
		sprite->setTexture(*(this->textures_tiles[(*tokens)[2]]));

		sf::Vector2f viewport_coord;
		
		// logical tile coordinate to screen coordinate conversion
		viewport_coord.x = map_coord.x * Settings::Instance()->TILE_WIDTH + (map_coord.y & 1) * (Settings::Instance()->TILE_WIDTH / 2);
		viewport_coord.y = map_coord.y * (Settings::Instance()->TILE_HEIGHT_RHOMBUS / 2) - Settings::Instance()->TILE_HEIGHT_OVERLAP;

		sprite->setPosition(viewport_coord);
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
	std::map<sf::Vector2i, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		window.draw(*iterator->second);
	}
}

std::string Map::to_string() {
	std::string output;

	std::map<sf::Vector2i, sf::Sprite*>::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		output += "Map (" + std::to_string(iterator->first.x) + ", " + std::to_string(iterator->first.y) + ")\n";
	}

	return output;
}