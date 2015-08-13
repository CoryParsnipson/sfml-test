#ifndef SCREENWRITER_H
#define SCREENWRITER_H

#include "../dependencies.h"

class ScreenWriter {
public:
   enum ALIGNMENT {
      LEFT,
      RIGHT,
      CENTER
   };

	ScreenWriter(sf::RenderWindow& window);
	~ScreenWriter();

	void load_font(std::string font_name, std::string filename);
	void write(std::string msg);
	void write(std::string msg, sf::Vector2i pos);

	//void set_font();
	void set_font_size(unsigned int font_size);
	//void set_font_style();
	
   void set_alignment(ScreenWriter::ALIGNMENT alignment);

protected:
	sf::RenderWindow& window;

	unsigned int font_size;
   ScreenWriter::ALIGNMENT alignment;
	std::string active_font;
	std::map<std::string, sf::Font*> fonts;
};

#endif
