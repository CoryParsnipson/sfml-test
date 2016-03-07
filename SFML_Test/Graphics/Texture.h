#ifndef TEXTURE_H
#define TEXTURE_H

#include "dependencies.h"

class Texture {
public:
   Texture(const std::string& id, const std::string& filename, sf::IntRect texture_area = sf::IntRect());
   ~Texture();

   const std::string& id();

   sf::Vector2f get_size();
   sf::Texture& get_texture();

   std::string to_string();
private:
   std::string id_;
   std::string filename_;

   sf::Image texture_mask_;
   sf::Texture texture_;
};

#endif
