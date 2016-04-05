#ifndef CAMERA_H
#define CAMERA_H

#include "Draw.h"
#include "MouseUtil.h"

class Camera
: public Draw
, public MouseControllable
{
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;

   Camera(const std::string& id, const sf::Vector2f& size);
   virtual ~Camera();

   const std::string& id();
   const std::string& to_string();

   void reset_pan();
   void reset_zoom();

   sf::Vector2f get_pan_delta();

   sf::View& get_view(); // TODO: get rid of this if possible

   // draw interface
   virtual void draw(RenderTarget& surface);

   // view interface
   sf::Vector2f get_size();
   void set_size(const sf::Vector2f& size);

   const sf::Vector2f& get_center();
   void set_center(const sf::Vector2f& center);

   // viewport management
   void set_viewport(const sf::FloatRect& viewport);
   const sf::FloatRect& get_viewport();

   // mouse control interface
   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButtonCommand& c);

protected:
   std::string id_;
   float zoom_factor_;

   sf::Vector2f original_center_;
   sf::View* view_;
};
#endif
