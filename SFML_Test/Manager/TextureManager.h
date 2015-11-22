#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "../dependencies.h"

#include "../Entities/Texture.h"

class TextureManager 
{
public:
   static Texture null_texture;

   typedef std::map<std::string, Texture*> texture_map_t;

   Texture* get_texture(std::string name);
   Texture* create_texture(std::string name, std::string filename);

   // for debug
   std::string to_string();

protected:

   texture_map_t textures;
};

#endif
