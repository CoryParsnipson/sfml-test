#include "Texture.h"
#include "Tile.h"

Tile::Tile(sf::Vector2i& pos, Texture& t)
: map_coordinate(pos)
{
	this->sprite = new sf::Sprite;
	this->sprite->setTexture(t.get_texture());

	// this should go somewhere else I think
	float s_x = pos.x * Settings::Instance()->TILE_WIDTH + (pos.y & 1) * (Settings::Instance()->TILE_WIDTH / 2);
	float s_y = pos.y * (Settings::Instance()->TILE_HEIGHT_RHOMBUS / 2) - Settings::Instance()->TILE_HEIGHT_OVERLAP;
	sf::Vector2f screen_coordinate(s_x, s_y);

	this->sprite->setPosition(screen_coordinate);
	this->sprite->setScale(sf::Vector2f((float)Settings::Instance()->SCALE_X, (float)Settings::Instance()->SCALE_Y));
}

Tile::~Tile() {
	delete this->sprite;
}

void Tile::draw(sf::RenderWindow& window) {
	window.draw(*(this->sprite));
}
