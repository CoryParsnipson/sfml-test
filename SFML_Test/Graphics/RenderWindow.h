#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "RenderTarget.h"

class RenderWindow
: public RenderTarget
{
public:
   RenderWindow(const std::string& title, const sf::Vector2f& size);
   virtual ~RenderWindow();

   // render window interface
   bool poll_event(sf::Event& event);

   // render target interface
   void set_camera(Camera& camera);

   sf::Vector2f get_world_coord(const sf::Vector2i& point, Camera* camera = nullptr);
   sf::Vector2i get_screen_coord(const sf::Vector2f& point, Camera* camera = nullptr);

   // draw interface
   void draw(sf::Drawable& d, int layer = 0);

   // update interface
   virtual void update();
   void clear();
   void clear(const sf::Color& color);

protected:
   sf::RenderWindow surface_;
};

#endif
