#ifndef CANVAS_H
#define CANVAS_H

#include "RenderSurface.h"

// ----------------------------------------------------------------------------
// forward declaration
// ----------------------------------------------------------------------------
class Camera;

// ----------------------------------------------------------------------------
// Canvas
// ----------------------------------------------------------------------------
class Canvas
: public RenderSurface
{
public:
   Canvas(const std::string& title, const sf::Vector2f& size, int framerate_limit = 60);
   virtual ~Canvas();

   virtual sf::Vector2f size() const;

   virtual void draw(sf::Drawable& drawable, sf::RenderStates render_states = sf::RenderStates::Default);
   virtual void draw(Camera* camera, sf::RenderStates render_states = sf::RenderStates::Default);

   virtual void update();
   virtual void clear(const Color& color = Color(sf::Color::Black));

   // canvas interface
   void set_framerate_limit(int limit);
   void set_mouse_cursor_visible(bool visible);

   bool poll_event(sf::Event& event);

protected:
   sf::RenderWindow surface_;
};

#endif
