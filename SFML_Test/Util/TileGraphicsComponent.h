#ifndef TILE_GRAPHICS_COMPONENT_H
#define TILE_GRAPHICS_COMPONENT_H

#include "dependencies.h"

#include "../Entities/Texture.h"
#include "GraphicsComponent.h"

class TileGraphicsComponent : public GraphicsComponent {
public:
   TileGraphicsComponent(Texture& t);

   void set_position(sf::Vector2i pos);
   void set_texture(Texture& t);

   virtual void update(Entity* entity, Graphics& graphics);

protected:
   void init();

   sf::Sprite sprite;
};

#endif
