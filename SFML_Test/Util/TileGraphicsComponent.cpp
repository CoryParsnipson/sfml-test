#include "TileGraphicsComponent.h"

#include "../Entities/Tile.h"

TileGraphicsComponent::TileGraphicsComponent(Texture& t)
{
   this->init();
   this->set_texture(t);
}

void TileGraphicsComponent::update(Entity* entity, Graphics& graphics) {
   Tile* tile = dynamic_cast<Tile*>(entity);

   this->set_position(tile->get_position());

   graphics.draw(this->sprite);
}

void TileGraphicsComponent::init() {
	this->sprite.setScale(sf::Vector2f((float)Settings::Instance()->SCALE_X, (float)Settings::Instance()->SCALE_Y));
}

void TileGraphicsComponent::set_position(sf::Vector2i pos) {
	float s_x = pos.x * Settings::Instance()->TILE_WIDTH + (pos.y & 1) * (Settings::Instance()->TILE_WIDTH / 2);
	float s_y = pos.y * (Settings::Instance()->TILE_HEIGHT_RHOMBUS / 2) - Settings::Instance()->TILE_HEIGHT_OVERLAP;
	sf::Vector2f screen_coordinate(s_x, s_y);

	this->sprite.setPosition(screen_coordinate);
}

void TileGraphicsComponent::set_texture(Texture& t) {
   this->sprite.setTexture(t.get_texture());
}
