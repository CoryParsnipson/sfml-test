#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "dependencies.h"

#include "Update.h"

class Layer;

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
   sf::Vector2f get_world_coord(const sf::Vector2i& point, Layer* layer = nullptr);

   // given an absolute world coordinate, get its screen coordinate
   sf::Vector2i get_screen_coord(const sf::Vector2f& point, Layer* layer = nullptr);

   void set_active_layer(Layer& layer); // all draw operations with unspecified layer will use this from now on

   // draw interface
   void draw(sf::Drawable& d);
   void draw(sf::Drawable& d, Layer& layer); // sets the current layer to provided layer

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);
   
protected:
   sf::RenderWindow* window;
};

#endif
