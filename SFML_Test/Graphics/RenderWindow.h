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
   void set_active_layer(Layer& layer);

   sf::Vector2f get_world_coord(const sf::Vector2i& point, Layer* layer = nullptr);
   sf::Vector2i get_screen_coord(const sf::Vector2f& point, Layer* layer = nullptr);

   // draw interface
   void draw(sf::Drawable& d);
   void draw(sf::Drawable& d, Layer& layer);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);
   void clear();
   void clear(const sf::Color& color);

protected:
   sf::RenderWindow surface_;
};

#endif
