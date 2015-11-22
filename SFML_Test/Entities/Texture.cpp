#include "Texture.h"

Texture::Texture(std::string name, std::string filename)
: name(name)
, filename(filename)
{
   this->texture_mask.loadFromFile(filename);
   this->texture_mask.createMaskFromColor(sf::Color::Magenta);

   this->texture.loadFromImage(this->texture_mask);
}

sf::Texture& Texture::get_texture() {
   return this->texture;
}

std::string Texture::to_string() {
   return "[Texture] name = " + this->name + ", filename = " + this->filename;
}
