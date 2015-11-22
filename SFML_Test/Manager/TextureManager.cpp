#include "TextureManager.h"

Texture* TextureManager::get_texture(std::string name) {
   texture_map_t::iterator item = this->textures.find(name);

   if (item != this->textures.end()) {
      return item->second;
   }

   return nullptr;
}

Texture* TextureManager::create_texture(std::string name, std::string filename) {
   this->textures.erase(name); // delete texture if one already exists at key name
   this->textures[name] = new Texture(name, filename);
   return this->textures[name];
}

std::string TextureManager::to_string() {
   std::string tm;
   texture_map_t::iterator t;

   tm = "[TextureManager] contents:\n";
   
   for(t = this->textures.begin(); t != this->textures.end(); t++) {
      tm += "  " + t->first + ": " + t->second->to_string() + "\n";
   }

   tm += "[TextureManager] end contents.\n";
   return tm;
}
