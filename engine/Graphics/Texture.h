#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Texture;

// ----------------------------------------------------------------------------
// aliases
// ----------------------------------------------------------------------------
using TexturePtr = std::shared_ptr<Texture>;

// ----------------------------------------------------------------------------
// Texture
//
// This contains loaded assets like spritesheets and images.
// ----------------------------------------------------------------------------
class Texture {
public:
   Texture(std::string id, std::string filename, sf::IntRect texture_area = sf::IntRect());
   ~Texture();

   const std::string& id();

   sf::Vector2f get_size();
   sf::Texture& get_texture();

   std::string to_string();
private:
   std::string id_;
   std::string filename_;

   sf::Texture texture_;
};

#endif
