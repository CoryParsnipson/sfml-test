#include "TextureAtlas.h"

TextureAtlas::TextureAtlas()
: Atlas("TextureAtlas")
{
}

void TextureAtlas::load(std::string key, std::string filename, sf::IntRect area /* = sf::IntRect() */) {
   this->set(key, new Texture(key, filename, area));
}

void TextureAtlas::unload(std::string key) {
   this->set(key, nullptr);
}

std::string TextureAtlas::to_string() const {
   std::string output = "[Atlas: " + this->id() + "<std::string, Texture>]\n";

   typename ItemList::const_iterator it = this->iterator();
   for (it = this->begin(); it != this->end(); ++it) {
      output += "  [" + it->first + " => " + it->second->to_string() + "]\n";
   }
   output += "[Atlas: end " + this->id() + " items]\n";
   
   return output;
}

Texture* TextureAtlas::get_post(std::string k, Texture* item) const {
   if (!item) {
      Service::get_logger().msg(this->id(), Logger::WARNING, "Cannot find texture with key '" + k + "'. Returning nullptr");
   }
   return item;
}
