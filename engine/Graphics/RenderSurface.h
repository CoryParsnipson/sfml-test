#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include "sfml.h"

class RenderSurface {
public:
   RenderSurface() {}
   virtual ~RenderSurface() {}

   virtual sf::Vector2f size() const = 0;

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
