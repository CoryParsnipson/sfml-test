#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics.hpp>

#include "Serializable.h"

// ----------------------------------------------------------------------------
// Font
//
// This is a wrapper class for sf::Font that also contains some helper
// functions for serialization, etc.
// ----------------------------------------------------------------------------
class Font final {
public:
   Font(const std::string& id, const std::string& filename);
   Font(const Font& other);
   ~Font();

   Font& operator=(const Font& other);
   void swap(Font& other);

   const std::string& id() const;
   void id(const std::string& id);

   bool load_from_file(const std::string& filename);

   const sf::Glyph& glyph(sf::Uint32 code_point, unsigned int character_size, bool bold) const;
   int kerning(sf::Uint32 first, sf::Uint32 second, unsigned int character_size) const;
   int line_spacing(unsigned int character_size) const;
   
   const sf::Texture& texture(unsigned int character_size) const;

   const sf::Font& get_font() const;

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

protected:
   std::string id_;
   std::string filename_;
   sf::Font font_;
};

#endif
