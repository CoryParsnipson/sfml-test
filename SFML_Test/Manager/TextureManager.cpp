#include "TextureManager.h"

// initialize static members
Texture* TextureManager::null_texture = new Texture("null", "");

Texture& TextureManager::get_texture(std::string id) {
   texture_map_t::iterator item = this->textures.find(id);

   if (item != this->textures.end()) {
      return *item->second;
   }

   return *TextureManager::null_texture;
}

Texture& TextureManager::create_texture(std::string id, std::string filename, sf::IntRect texture_area) {
   this->textures.erase(id); // delete texture if one already exists at key name
   this->textures[id] = new Texture(id, filename, texture_area);
   return *this->textures[id];
}

void TextureManager::print() {
   texture_map_t::iterator t;

   Service::get_logger().msg("TextureManager", Logger::INFO, "contents:");

   for(t = this->textures.begin(); t != this->textures.end(); t++) {
      Service::get_logger().msg("TextureManager", Logger::INFO, "  " + t->first + ": " + t->second->to_string());
   }

   Service::get_logger().msg("TextureManager", Logger::INFO, "end contents.");
}
