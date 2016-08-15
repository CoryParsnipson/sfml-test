#ifndef TEXTURE_DICTIONARY_H
#define TEXTURE_DICTIONARY_H

#include "dependencies.h"
#include "Dictionary.h"
#include "Texture.h"

// ----------------------------------------------------------------------------
// TextureDictionary
//
// This is a map that contains Texture pointers.
// ----------------------------------------------------------------------------
class TextureDictionary : public Dictionary<std::string, Texture> {
public:
   TextureDictionary();
   
   void load(std::string key, std::string filename, sf::IntRect area = sf::IntRect());
   void unload(std::string key);

protected:
   virtual std::string to_string() const;
   Texture* get_post(std::string k, Texture* item) const;
};

#endif
