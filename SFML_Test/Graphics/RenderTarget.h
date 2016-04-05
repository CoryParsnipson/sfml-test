#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "dependencies.h"

class Game;
class Scene;
class Entity;
class Camera;

class RenderTarget {
public:
   // all draw operations with unspecified layer will use this from now on
   virtual void set_camera(Camera& camera) = 0;

   // given a point on the screen, get the absolute world coordinate (apply global transforms)
   virtual sf::Vector2f get_world_coord(const sf::Vector2i& point, Camera* camera = nullptr) = 0;

   // given an absolute world coordinate, get its screen coordinate
   virtual sf::Vector2i get_screen_coord(const sf::Vector2f& point, Camera* camera = nullptr) = 0;

   // draw interface
   virtual void draw(sf::Drawable& d, int layer = 0) = 0;

   // update interface
   virtual void update() = 0;
   virtual void clear() = 0;
   virtual void clear(const sf::Color& color) = 0;
};

#endif
