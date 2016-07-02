#ifndef SUB_CANVAS_H
#define SUB_CANVAS_H

#include "RenderSurface.h"

class Camera;

class SubCanvas
: public RenderSurface
{
public:
   SubCanvas(sf::FloatRect bounds);
   SubCanvas(const sf::Vector2f& size);
   virtual ~SubCanvas();

   // render surface interface
   virtual void set_camera(Camera& camera);

   virtual void draw(sf::Drawable& drawable, sf::RenderStates render_states = sf::RenderStates::Default);

   virtual void update();
   virtual void clear(const sf::Color& color = sf::Color::Black);

   // sub canvas interface
   const sf::Texture& get_texture();

   void offset(sf::Vector2f offset);
   void offset(float offset_x, float offset_y);

   const sf::FloatRect& bounds();
   void bounds(sf::FloatRect bounds);
   void bounds(sf::Vector2f pos, sf::Vector2f size);
   void bounds(float left, float top, float width, float height);

protected:
   sf::FloatRect bounds_;
   sf::Transform offset_;
   sf::RenderTexture surface_;
};

#endif
