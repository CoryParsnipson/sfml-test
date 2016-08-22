#ifndef TEXTURE_DICTIONARY_H
#define TEXTURE_DICTIONARY_H

#include "dependencies.h"
#include "Atlas.h"
#include "Texture.h"

// ----------------------------------------------------------------------------
// TextureAtlas
//
// This is a map that contains Texture pointers.
// ----------------------------------------------------------------------------
class TextureAtlas : public Atlas<std::string, Texture> {
public:
   TextureAtlas();
   
   void load(std::string key, std::string filename, sf::IntRect area = sf::IntRect());
   void unload(std::string key);

protected:
   virtual std::string to_string() const;
   Texture* get_post(std::string k, Texture* item) const;
};

#endif
