#ifndef FONT_DICTIONARY_H
#define FONT_DICTIONARY_H

#include <string>
#include <SFML/Graphics.hpp>

#include "Font.h"
#include "Atlas.h"
#include "Serializable.h"

// ----------------------------------------------------------------------------
// FontAtlas
//
// This is a map that contains Font pointers.
// ----------------------------------------------------------------------------
class FontAtlas
: public Atlas<std::string, Font>
, public Serializable
{
public:
   FontAtlas();

   void load(std::string font_key, std::string font_filename);
   void unload(std::string font_key);

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);
};

#endif
