#include "../dependencies.h"
#include "ScreenWriter.h"

#include "../Util/Graphics.h"

ScreenWriter::ScreenWriter()
: font_size(12)
, alignment(ScreenWriter::ALIGNMENT::LEFT)
, active_font("")
{
}

ScreenWriter::~ScreenWriter() {
	std::map<std::string, sf::Font*>::const_iterator it;
	for (it = this->fonts.begin(); it != this->fonts.end(); it++) {
		delete it->second;
	}
}

void ScreenWriter::load_font(std::string font_name, std::string filename) {
	sf::Font* font = new sf::Font();

	if (!font->loadFromFile(filename)) {
		std::cout << "Error loading font " << filename << "." << std::endl;
		return;
	}

	this->fonts[font_name] = font;

	// set the current font to the active font if there is no value
	if (this->active_font.compare("") == 0) {
		this->active_font = font_name;
	}
}

void ScreenWriter::write(Viewport& viewport, std::string msg) {
	this->write(viewport, msg, sf::Vector2i(0, 0));
}

void ScreenWriter::write(Viewport& viewport, std::string msg, sf::Vector2i pos) {
	sf::Text t;
	t.setString(msg);
	t.setFont(*(this->fonts[this->active_font]));
	t.setCharacterSize(this->font_size);
	t.setColor(sf::Color::White);

   sf::Vector2f alignment_offset(0.0, 0.0);
   if (this->alignment == ScreenWriter::ALIGNMENT::CENTER) {
      sf::FloatRect tb = t.getLocalBounds();
      alignment_offset.x = tb.left + tb.width / 2.0f;
      alignment_offset.y = tb.top + tb.height / 2.0f;
   }
   else if (this->alignment == ScreenWriter::ALIGNMENT::RIGHT) {
      sf::FloatRect tb = t.getLocalBounds();
      alignment_offset.x = tb.width;
      alignment_offset.y = tb.top + tb.height / 2.0f;
   }
   t.setOrigin(alignment_offset);
	t.setPosition(static_cast<sf::Vector2f>(pos));

   viewport.draw(t);
}

void ScreenWriter::set_font_size(unsigned int font_size) {
   this->font_size = font_size;
}

void ScreenWriter::set_alignment(ScreenWriter::ALIGNMENT alignment) {
   this->alignment = alignment;
}
