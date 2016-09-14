#ifndef FONT_DICTIONARY_H
#define FONT_DICTIONARY_H

#include <string>

#include "sfml.h"
#include "Atlas.h"

// ----------------------------------------------------------------------------
// FontAtlas
//
// This is a map that contains sf::Font pointers.
// ----------------------------------------------------------------------------
class FontAtlas : public Atlas<std::string, sf::Font> {
public:
   FontAtlas();

   void load(std::string font_key, std::string font_filename);
   void unload(std::string font_key);
};

#endif
