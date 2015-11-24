#include "Tile.h"

Tile::Tile(GraphicsComponent* graphics)
: Entity(graphics)
{
}

Tile::~Tile() {
}

void Tile::set_position(sf::Vector2i pos) {
   this->map_coordinate = pos;
}

sf::Vector2i Tile::get_position() {
   return this->map_coordinate;
}

void Tile::update(Viewport& viewport) {
   this->graphics_->update(this, viewport);
}
