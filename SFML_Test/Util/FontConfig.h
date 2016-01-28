#ifndef FONT_CONFIG_H
#define FONT_CONFIG_H

#include "dependencies.h"

class FontConfig {
public:
   enum ALIGN {
      LEFT,
      RIGHT,
      CENTER
   };

   // config members
   std::string font;
   int size;
   ALIGN alignment;
   sf::Color color;
   
   // style? (italics, bold, underline, etc)

   FontConfig(
      std::string font,
      int size = 12,
      ALIGN alignment = ALIGN::LEFT,
      sf::Color color = sf::Color::White
   )
   : font(font)
   , size(size)
   , alignment(alignment)
   , color(color) {}
};

#endif
