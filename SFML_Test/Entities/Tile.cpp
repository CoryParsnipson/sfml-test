#include "Tile.h"
#include "TileGraphicsComponent.h"

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

bool Tile::contains(sf::Vector2i coord) {
   if (!this->graphics_) {
      return false;
   }

   return dynamic_cast<TileGraphicsComponent*>(this->graphics_)->contains(coord);
}

std::string Tile::to_string() {
   return "[Tile: (" + std::to_string(this->map_coordinate.x) + ", " + std::to_string(this->map_coordinate.y) + ")]";
}

void Tile::update(Viewport& viewport) {
   this->graphics_->update(this, viewport);
}
