#include "TextureDictionary.h"

TextureDictionary::TextureDictionary()
: Dictionary("TextureDictionary")
{
}

void TextureDictionary::load(std::string key, std::string filename, sf::IntRect area /* = sf::IntRect() */) {
   this->set(key, new Texture(key, filename, area));
}

void TextureDictionary::unload(std::string key) {
   this->set(key, nullptr);
}

std::string TextureDictionary::to_string() const {
   std::string output = "[Dictionary: " + this->id() + "<std::string, Texture>]\n";

   typename ItemList::const_iterator it = this->iterator();
   for (it = this->begin(); it != this->end(); ++it) {
      output += "  [" + it->first + " => " + it->second->to_string() + "]\n";
   }
   output += "[Dictionary: end " + this->id() + " items]\n";
   
   return output;
}

Texture* TextureDictionary::get_post(std::string k, Texture* item) const {
   if (!item) {
      Service::get_logger().msg(this->id(), Logger::WARNING, "Cannot find texture with key '" + k + "'. Returning nullptr");
   }
   return item;
}
