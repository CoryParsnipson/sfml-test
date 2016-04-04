#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "dependencies.h"

class Game;
class Layer;
class Scene;
class Entity;

class RenderTarget {
public:
   // all draw operations with unspecified layer will use this from now on
   virtual void set_active_layer(Layer& layer) = 0;

   // given a point on the screen, get the absolute world coordinate (apply global transforms)
   virtual sf::Vector2f get_world_coord(const sf::Vector2i& point, Layer* layer = nullptr) = 0;

   // given an absolute world coordinate, get its screen coordinate
   virtual sf::Vector2i get_screen_coord(const sf::Vector2f& point, Layer* layer = nullptr) = 0;

   // draw interface
   virtual void draw(sf::Drawable& d) = 0;
   virtual void draw(sf::Drawable& d, Layer& layer) = 0; // sets the current layer to provided layer

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;
   virtual void clear() = 0;
   virtual void clear(const sf::Color& color) = 0;
};

#endif
