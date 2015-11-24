#ifndef SCREENWRITER_H
#define SCREENWRITER_H

#include "../dependencies.h"
#include "Viewport.h"

class ScreenWriter {
public:
   enum ALIGNMENT {
      LEFT,
      RIGHT,
      CENTER
   };

	ScreenWriter();
	~ScreenWriter();

	void load_font(std::string font_name, std::string filename);
	void write(Viewport& viewport, std::string msg);
	void write(Viewport& viewport, std::string msg, sf::Vector2i pos);

	//void set_font();
	void set_font_size(unsigned int font_size);
	//void set_font_style();
	
   void set_alignment(ScreenWriter::ALIGNMENT alignment);

protected:
	unsigned int font_size;
   ScreenWriter::ALIGNMENT alignment;
	std::string active_font;
	std::map<std::string, sf::Font*> fonts;
};

#endif
