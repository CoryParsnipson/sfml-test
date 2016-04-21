#ifndef SUB_CANVAS_H
#define SUB_CANVAS_H

#include "RenderSurface.h"

class Camera;

class SubCanvas
: public RenderSurface
{
public:
   SubCanvas(const sf::Vector2f& size);
   virtual ~SubCanvas();

   // render surface interface
   virtual void set_camera(Camera& camera);

   virtual void draw(sf::Drawable& drawable, sf::RenderStates render_states = sf::RenderStates::Default);

   virtual void update();
   virtual void clear(const sf::Color& color = sf::Color::Black);

   // sub canvas interface
   const sf::Texture& get_texture();

protected:
   sf::RenderTexture surface_;
};

#endif
