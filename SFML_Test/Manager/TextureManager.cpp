#include "TextureManager.h"

Texture* TextureManager::get_texture(int id) {
   texture_map_t::iterator item = this->textures.find(id);

   if (item != this->textures.end()) {
      return item->second;
   }

   return nullptr;
}

Texture* TextureManager::create_texture(int id, std::string filename) {
   this->textures.erase(id); // delete texture if one already exists at key name
   this->textures[id] = new Texture(filename);
   return this->textures[id];
}

void TextureManager::print() {
   texture_map_t::iterator t;

   Service::get_logger().msg("TextureManager", Logger::INFO, "contents:");

   for(t = this->textures.begin(); t != this->textures.end(); t++) {
      Service::get_logger().msg("TextureManager", Logger::INFO, "  " + std::to_string(t->first) + ": " + t->second->to_string());
   }

   Service::get_logger().msg("TextureManager", Logger::INFO, "end contents.");
}
