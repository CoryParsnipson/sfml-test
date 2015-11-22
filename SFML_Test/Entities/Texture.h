#ifndef TEXTURE_H
#define TEXTURE_H

#include "../dependencies.h"

class Texture {
public:
   Texture(std::string name, std::string filename);
   ~Texture();

   sf::Texture& get_texture();

   std::string to_string();
private:
   std::string name;
   std::string filename;

   sf::Image texture_mask;
   sf::Texture texture;
};

#endif
