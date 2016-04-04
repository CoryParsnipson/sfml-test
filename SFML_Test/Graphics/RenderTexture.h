#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "RenderTarget.h"

class RenderTexture
: public RenderTarget
{
public:
   RenderTexture();
   virtual ~RenderTexture();

   // render texture interface
   const sf::Texture& get_texture() const;

   // render target interface
   void set_active_layer(Layer& layer);

   sf::Vector2f get_world_coord(const sf::Vector2i& point, Layer* layer = nullptr);
   sf::Vector2i get_screen_coord(const sf::Vector2f& point, Layer* layer = nullptr);

   // draw interface
   void draw(sf::Drawable& d);
   void draw(sf::Drawable& d, Layer& layer);

   // update interface
   virtual void update();
   void clear();
   void clear(const sf::Color& color);

protected:
   sf::RenderTexture surface_;
};

#endif
