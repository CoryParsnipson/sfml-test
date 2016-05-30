#ifndef CAMERA_H
#define CAMERA_H

#include "Draw.h"
#include "MouseControllable.h"

class Camera
: public Draw
, public MouseControllable
{
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;

   // TODO: is there a way to access view without having to explicitly declare every friend?
   friend class RenderSurface;
   friend class Canvas;
   friend class SubCanvas;

   Camera(const std::string& id, const sf::Vector2f& size);
   virtual ~Camera();

   const std::string& id();
   const std::string& to_string();

   void reset_pan();
   void reset_zoom();

   sf::Vector2f get_pan_delta();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // view interface
   sf::Vector2f get_size();
   void set_size(const sf::Vector2f& size);

   const sf::Vector2f& get_center();
   void set_center(const sf::Vector2f& center);

   const sf::Transform& get_transform() const;

   void move(sf::Vector2f delta);

   // viewport management
   void set_viewport(const sf::FloatRect& viewport);
   const sf::FloatRect& get_viewport();

   // mouse control interface
   virtual void drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButton button, MouseButtonState state, sf::Vector2f pos);

protected:
   std::string id_;
   float zoom_factor_;

   sf::Vector2f original_center_;
   sf::Transform state_; // sf::Transform that is modified alongside sf::View
   sf::View* view_;
};
#endif
