#include "Texture.h"

Texture::Texture(std::string id, std::string filename, sf::IntRect texture_area)
: id_(id)
, filename_(filename)
{
   this->texture_.loadFromFile(filename, texture_area);
}

Texture::~Texture() {
}

const std::string& Texture::id() {
   return this->id_;
}

sf::Vector2f Texture::get_size() {
   return static_cast<sf::Vector2f>(this->texture_.getSize());
}

sf::Texture& Texture::get_texture() {
   return this->texture_;
}

std::string Texture::to_string() {
   return "[Texture] filename = " + this->filename_;
}
