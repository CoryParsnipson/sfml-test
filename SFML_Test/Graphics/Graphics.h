#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "dependencies.h"

#include "Update.h"

class Graphics
: public Update
{
public:
   Graphics();
   Graphics(sf::RenderWindow& window);

   bool is_open();
   void close();
   void clear();

   sf::RenderWindow& get_window();

   // given a point on the screen, get the absolute world coordinate (apply global transforms)
   sf::Vector2f get_world_coord(const sf::Vector2i& point, sf::View* view = nullptr);

   // given an absolute world coordinate, get its screen coordinate
   sf::Vector2i get_screen_coord(const sf::Vector2f& point, sf::View* view = nullptr);

   // draw interface
   virtual void draw(sf::Drawable& d, sf::View& view);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);
   
protected:
   sf::RenderWindow* window;
};

#endif
