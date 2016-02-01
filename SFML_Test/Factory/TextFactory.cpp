#include "TextFactory.h"

#include "GraphicsPart.h"

// initialize static members
TextFactory* TextFactory::inst_ = new TextFactory();

TextFactory* TextFactory::inst() {
   return TextFactory::inst_;
}

TextFactory::TextFactory() {
}

TextFactory::~TextFactory() {
   FontList::const_iterator it;
	for (it = this->fonts_.begin(); it != this->fonts_.end(); it++) {
		delete it->second;
	}
   this->fonts_.clear();
}

void TextFactory::load_font(std::string font_name, std::string filename) {
	sf::Font* font = new sf::Font();

	if (!font->loadFromFile(filename)) {
      Service::get_logger().msg("TextFactory", Logger::ERROR, "Cannot load font '" + filename + "'.");
		return;
	}

	this->fonts_[font_name] = font;
}

void TextFactory::unload_font(std::string font_name) {
   FontList::const_iterator it = this->fonts_.find(font_name);

   if (it == this->fonts_.end()) {
      Service::get_logger().msg("TextFactory", Logger::WARNING, "Font '" + font_name + "' not found. Could not unload.");
      return;
   }

   this->fonts_.erase(it);
}

sf::Font* TextFactory::get_font(std::string font_name) {
   FontList::const_iterator it = this->fonts_.find(font_name);

   if (it == this->fonts_.end()) {
      return nullptr;
   }

   return it->second;
}

Entity* TextFactory::create_text(std::string msg, sf::Font* font, sf::Vector2f pos, int size, ALIGN alignment, sf::Color color) {
   Entity* text = new Entity();
   GraphicsPart* text_graphics = new GraphicsPart();

   sf::Text* t = new sf::Text();

   if (font) {
      t->setFont(*font);
   }

   t->setString(msg);
   t->setCharacterSize(size);
   t->setColor(color);
   t->setOrigin(this->get_origin_from_alignment(*t, alignment));
   t->setPosition(pos);

   text_graphics->add(t);
   text->add(text_graphics);

   return text;
}

Entity* TextFactory::create_text(std::string msg, std::string font, sf::Vector2f pos, int size, ALIGN alignment, sf::Color color) {
   return this->create_text(msg, this->get_font(font), pos, size, alignment, color);
}

sf::Vector2f TextFactory::get_origin_from_alignment(sf::Text& t, ALIGN alignment) {
   sf::FloatRect text_boundary = t.getLocalBounds();
   sf::Vector2f origin(0, 0);

   switch (alignment) {
   case (ALIGN::CENTER) :
      origin.x = text_boundary.left + text_boundary.width / 2.0f;
      origin.y = text_boundary.top + text_boundary.height / 2.0f;
      break;
   case (ALIGN::RIGHT) :
      origin.x = text_boundary.width;
      origin.y = text_boundary.top + text_boundary.height / 2.0f;
      break;
   default :
      origin.x = text_boundary.left;
      origin.y = text_boundary.top;
   }

   return origin;
}
