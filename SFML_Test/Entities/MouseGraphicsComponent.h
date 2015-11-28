#ifndef MOUSE_GRAPHICS_COMPONENT_H
#define MOUSE_GRAPHICS_COMPONENT_H

#include "dependencies.h"

#include "FontConfig.h"
#include "GraphicsComponent.h"

class MouseGraphicsComponent : public GraphicsComponent {
public:
   MouseGraphicsComponent();
   
   virtual void update(Entity* entity, Viewport& viewport);

protected:
   FontConfig font_cursor;
   sf::RectangleShape cursor;

   void set_position(sf::Vector2f pos);
   void set_position(sf::Vector2i pos);
};

#endif
