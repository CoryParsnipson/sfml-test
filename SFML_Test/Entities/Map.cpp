#include "Map.h"

#include "../Game.h"
#include "../FileReader/MapFileReader.h"

#include "TileGraphicsComponent.h"

Map::Map() {
}

Map::~Map() {
   // need to clean up all Tile entities
	map_type_t::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
		delete iterator->second;
	}
}

void Map::load_mapfile(Game& game, std::string map_filename) {
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

      // create Tile entity
      GraphicsComponent* tile_gc = new TileGraphicsComponent(*(game.texture_manager.get_texture((*tokens)[2])));
      this->tiles[map_coord] = new Tile(tile_gc);
      this->tiles[map_coord]->set_position(map_coord);
      
		tokens->clear();
	}
	delete tokens;
}

void Map::draw(Viewport& viewport) {
	map_type_t::const_iterator iterator;
	for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
      iterator->second->update(viewport);
	}
}

std::string Map::to_string() {
	std::string output;

	map_type_t::const_iterator iterator;
	//for (iterator = this->tiles.begin(); iterator != this->tiles.end(); iterator++) {
	//	output += "Map (" + std::to_string(iterator->first.x) + ", " + std::to_string(iterator->first.y) + ")\n";
	//}

	return output;
}
