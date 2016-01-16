#include "Texture.h"

Texture::Texture(std::string filename, sf::IntRect texture_area)
: filename(filename)
{
   this->texture_mask.loadFromFile(filename);
   this->texture_mask.createMaskFromColor(sf::Color::Magenta);

   this->texture.loadFromImage(this->texture_mask, texture_area);
}

sf::Vector2f Texture::get_size() {
   return static_cast<sf::Vector2f>(this->texture.getSize());
}

sf::Texture& Texture::get_texture() {
   return this->texture;
}

std::string Texture::to_string() {
   return "[Texture] filename = " + this->filename;
}
