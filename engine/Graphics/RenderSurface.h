#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include <SFML/Graphics.hpp>

#include "Color.h"
#include "Camera.h"

// ----------------------------------------------------------------------------
// RenderSurface
//
// This is an abstract interface to define an object that represents video
// memory.
// ----------------------------------------------------------------------------
class RenderSurface {
public:
   RenderSurface() {}
   virtual ~RenderSurface() {}

   virtual sf::Vector2f size() const = 0;

   // draw interface
   virtual void draw(sf::Drawable& drawable, sf::RenderStates render_states = sf::RenderStates::Default) = 0;
   virtual void draw(Camera* camera, sf::RenderStates render_states = sf::RenderStates::Default) = 0;

   // update interface
   virtual void update() = 0;
   virtual void clear(const Color& color = Color(sf::Color::Black)) = 0;

protected:
   virtual void set_view(Camera* camera, sf::RenderWindow& window, sf::RenderStates render_states = sf::RenderStates::Default) {
      window.setView(camera->view_);
   }
};

#endif
