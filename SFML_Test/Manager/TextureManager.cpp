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

std::string TextureManager::to_string() {
   std::string tm;
   texture_map_t::iterator t;

   tm = "[TextureManager] contents:\n";
   
   for(t = this->textures.begin(); t != this->textures.end(); t++) {
      tm += "  " + std::to_string(t->first) + ": " + t->second->to_string() + "\n";
   }

   tm += "[TextureManager] end contents.\n";
   return tm;
}
