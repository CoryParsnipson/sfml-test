#ifndef TILE_GRAPHICS_COMPONENT_H
#define TILE_GRAPHICS_COMPONENT_H

#include "dependencies.h"

#include "FontConfig.h"
#include "../Entities/Texture.h"
#include "GraphicsComponent.h"

class TileGraphicsComponent : public GraphicsComponent {
public:
   TileGraphicsComponent(Texture& t);

   void set_position(sf::Vector2i pos);
   void set_texture(Texture& t);

   bool contains(sf::Vector2i coord);

   virtual void update(Entity* entity, Viewport& viewport);

protected:
   void init();
   
   FontConfig font_info; 

   sf::Sprite sprite;
};

#endif
