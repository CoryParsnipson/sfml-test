#include "FontDictionary.h"

FontDictionary::FontDictionary()
: Dictionary("FontDictionary")
{
}

void FontDictionary::load(std::string font_key, std::string font_filename) {
   sf::Font* font = new sf::Font();

   if (!font->loadFromFile(font_filename)) {
      Service::get_logger().msg(this->id(), Logger::ERROR, "Cannot load font '" + font_filename + "'.");
      return;
   }
   
   this->set(font_key, font);
}

void FontDictionary::unload(std::string font_key) {
   this->set(font_key, nullptr);
}
