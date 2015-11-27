#include "GraphicsWriter.h"
#include "Graphics.h"

// initialize static members
FontConfig GraphicsWriter::default_config = FontConfig("retro");

GraphicsWriter::~GraphicsWriter() {
   font_map_t::const_iterator it;
	for (it = this->fonts.begin(); it != this->fonts.end(); it++) {
		delete it->second;
	}
}

void GraphicsWriter::load_font(std::string font_name, std::string filename) {
	sf::Font* font = new sf::Font();

	if (!font->loadFromFile(filename)) {
      // TODO: exception handling
		std::cout << "Error loading font " << filename << "." << std::endl;
		return;
	}

	this->fonts[font_name] = font;
}

void GraphicsWriter::unload_font(std::string font_name) {
   font_map_t::const_iterator it = this->fonts.find(font_name);

   if (it == this->fonts.end()) {
      // TODO: exception handling?
      std::cout << "(GraphicsWriter::unload_font) Warning: font " << font_name << " not found." << std::endl;
      return;
   }

   this->fonts.erase(it);
}

sf::Font* GraphicsWriter::get_font(std::string font_name) {
   font_map_t::const_iterator it = this->fonts.find(font_name);

   if (it == this->fonts.end()) {
      return nullptr;
   }

   return it->second;
}

sf::Vector2f GraphicsWriter::get_origin_from_alignment(sf::Text& t, const FontConfig* config) {
   sf::FloatRect text_boundary = t.getLocalBounds();
   sf::Vector2f origin(0.0, 0.0);
   switch (config->alignment) {
   case (FontConfig::ALIGN::CENTER) :
      origin.x = text_boundary.left + text_boundary.width / 2.0f;
      origin.y = text_boundary.top + text_boundary.height / 2.0f;
      break;
   case (FontConfig::ALIGN::RIGHT) :
      origin.x = text_boundary.width;
      origin.y = text_boundary.top + text_boundary.height / 2.0f;
      break;
   default :
      origin.x = text_boundary.left;
      origin.y = text_boundary.top;
   }

   return origin;
}

void GraphicsWriter::write(Graphics& graphics, std::string msg, sf::Vector2f pos, const FontConfig* config) {
   sf::Text t;
   sf::Font* font_name;

   t.setString(msg);

   const FontConfig* final_config = &GraphicsWriter::default_config;
   if (config) {
      final_config = config;
   }

   font_name = this->get_font(final_config->font);
   if (!font_name) {
      // TODO: exception handling
      std::cout << "(GraphicsWriter::write) Error: invalid font name " << final_config->font << " specified." << std::endl;
      return;
   }

   t.setFont(*font_name);
   t.setCharacterSize(final_config->size);
   t.setColor(final_config->color);
   t.setOrigin(this->get_origin_from_alignment(t, final_config));
   t.setPosition(pos);

   graphics.draw(t);
}
