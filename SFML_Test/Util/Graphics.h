#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "dependencies.h"

#include "../Entities/Viewport.h"

class Graphics {
public:
   Graphics();
   Graphics(sf::RenderWindow& window);

   bool is_open();
   void close();
   
   void update();
   void draw(sf::Drawable& d, Viewport* viewport = nullptr);
   void clear();

   sf::RenderWindow& get_window();

protected:
   sf::RenderWindow* window;
};

#endif
