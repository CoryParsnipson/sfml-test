#ifndef FONT_DICTIONARY_H
#define FONT_DICTIONARY_H

#include "dependencies.h"
#include "Dictionary.h"

// ----------------------------------------------------------------------------
// FontDictionary
//
// This is a map that contains sf::Font pointers.
// ----------------------------------------------------------------------------
class FontDictionary : public Dictionary<std::string, sf::Font> {
public:
   FontDictionary();

   void load(std::string font_key, std::string font_filename);
   void unload(std::string font_key);
};

#endif
