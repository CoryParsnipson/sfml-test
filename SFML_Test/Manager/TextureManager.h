#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "dependencies.h"

#include "Texture.h"

class TextureManager 
{
public:
   static Texture null_texture;

   typedef std::map<int, Texture*> texture_map_t;

   Texture* get_texture(int id);
   Texture* create_texture(int id, std::string filename);

   // for debug
   void print();

protected:

   texture_map_t textures;
};

#endif
