#include "TextFactory.h"

#include "Graphic.h"
#include "Entity.h"
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

Text* TextFactory::create_text(std::string msg, sf::Font* font, sf::Vector2f pos, int size, ALIGN alignment, sf::Color color) {
   Text* t = new Text();

   t->set_font(font);
   t->set_string(msg);
   t->set_character_size(size);
   t->set_color(color);
   
   sf::FloatRect bounds = t->get_local_bounds();
   t->set_origin(this->get_origin_from_alignment(bounds, alignment));

   t->set_position(pos);

   return t;
}

Text* TextFactory::create_text(std::string msg, std::string font, sf::Vector2f pos, int size, ALIGN alignment, sf::Color color) {
   return this->create_text(msg, this->get_font(font), pos, size, alignment, color);
}

Entity* TextFactory::create_text_entity(std::string msg, std::string font, sf::Vector2f pos, int size, ALIGN alignment, sf::Color color) {
   Entity* text_entity = new Entity();
   GraphicsPart* text_entity_graphics = new GraphicsPart();

   text_entity_graphics->add(this->create_text(msg, font, pos, size, alignment, color));
   text_entity->add(text_entity_graphics);

   return text_entity;
}

sf::Vector2f TextFactory::get_origin_from_alignment(sf::FloatRect& bounds, ALIGN alignment) {
   sf::Vector2f origin(0, 0);

   switch (alignment) {
   case (ALIGN::CENTER) :
      origin.x = bounds.left + bounds.width / 2.0f;
      origin.y = bounds.top + bounds.height / 2.0f;
      break;
   case (ALIGN::RIGHT) :
      origin.x = bounds.width;
      origin.y = bounds.top + bounds.height / 2.0f;
      break;
   default :
      origin.x = bounds.left;
      origin.y = bounds.top;
   }

   return origin;
}
