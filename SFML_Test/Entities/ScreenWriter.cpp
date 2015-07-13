#include "../dependencies.h"
#include "ScreenWriter.h"

ScreenWriter::ScreenWriter(sf::RenderWindow& window)
: window(window)
, font_size(12)
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

void ScreenWriter::write(std::string msg) {
	this->write(msg, sf::Vector2i(0, 0));
}

void ScreenWriter::write(std::string msg, sf::Vector2i pos) {
	sf::Text t;
	t.setString(msg);
	t.setFont(*(this->fonts[this->active_font]));
	t.setCharacterSize(this->font_size);
	t.setColor(sf::Color::White);

	t.setPosition(static_cast<sf::Vector2f>(pos));

	window.draw(t);
}