#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "dependencies.h"

#include "Texture.h"

class TextureManager 
{
public:
   typedef std::map<std::string, Texture*> texture_map_t;

   Texture& get_texture(std::string id);
   Texture& create_texture(std::string id, std::string filename, sf::IntRect texture_area = sf::IntRect());

   // for debug
   void print();

protected:
   texture_map_t textures;

   static Texture* null_texture;
};

#endif
