#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include "dependencies.h"
#include "Draw.h"

class Game;
class Scene;
class Entity;
class Camera;

class RenderSurface {
public:
   RenderSurface() {}
   virtual ~RenderSurface() {}

   // draw interface
   virtual void draw(sf::Drawable& drawable, sf::RenderStates render_states = sf::RenderStates::Default) = 0;

   // update interface
   virtual void update() = 0;
   virtual void clear(const sf::Color& color = sf::Color::Black) = 0;

   // for internal use only...
   virtual const sf::View& view() const = 0;
   virtual void view(const sf::View& view) = 0;
};

#endif
