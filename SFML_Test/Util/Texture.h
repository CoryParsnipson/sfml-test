#ifndef TEXTURE_H
#define TEXTURE_H

#include "dependencies.h"

class Texture {
public:
   Texture(std::string filename, sf::IntRect texture_area = sf::IntRect());
   ~Texture();

   sf::Texture& get_texture();

   std::string to_string();
private:
   std::string filename;

   sf::Image texture_mask;
   sf::Texture texture;
};

#endif
