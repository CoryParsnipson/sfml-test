#include "TileGraphicsComponent.h"

#include "Tile.h"

TileGraphicsComponent::TileGraphicsComponent(Texture& t)
: font_info("retro", 12, FontConfig::ALIGN::CENTER)
{
   this->init();
   this->set_texture(t);
}

void TileGraphicsComponent::update(Entity* entity, Viewport& viewport) {
   sf::Vector2i tile_pos;
   Tile* tile = dynamic_cast<Tile*>(entity);

   tile_pos = tile->get_position();
   this->set_position(tile_pos);
   viewport.draw(this->sprite);

   // draw coordinate information
   std::stringstream coordinate_info;
   coordinate_info << "(" << tile_pos.x << ", " << tile_pos.y << ")";

   sf::Vector2f sprite_pos = this->sprite.getPosition();
   sf::FloatRect sprite_rect = this->sprite.getLocalBounds();
   sf::Vector2f msg_pos;
   msg_pos.x = sprite_pos.x + (float)Settings::Instance()->SCALE_X * sprite_rect.width / 2.0f;
   msg_pos.y = sprite_pos.y + (float)Settings::Instance()->SCALE_Y * sprite_rect.height * 2.5f / 4.0f;

   viewport.write(coordinate_info.str(), msg_pos, &this->font_info);

   coordinate_info.str("");
   coordinate_info << "(" << msg_pos.x << ", " << msg_pos.y << ")";

   viewport.write(coordinate_info.str(), msg_pos + sf::Vector2f(0, 15), &this->font_info);
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
