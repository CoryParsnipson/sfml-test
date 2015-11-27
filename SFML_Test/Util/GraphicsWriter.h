#ifndef GRAPHICS_WRITER_H
#define GRAPHICS_WRITER_H

#include "../dependencies.h"
#include "FontConfig.h"

// forward declarations
class Graphics;

// draw text and text related stuff to the screen
class GraphicsWriter {
public:
   typedef std::map<std::string, sf::Font*> font_map_t;

   static FontConfig default_config;

   virtual ~GraphicsWriter();

   void load_font(std::string font_name, std::string filename);
   void unload_font(std::string font_name);
   sf::Font* get_font(std::string font_name);

   void write(Graphics& graphics, std::string msg, sf::Vector2f pos = sf::Vector2f(0, 0), const FontConfig* config = nullptr);

protected:
   font_map_t fonts;
   
   // protected helper functions
   sf::Vector2f get_origin_from_alignment(sf::Text& t, const FontConfig* config = nullptr);
};

#endif
