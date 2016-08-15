#ifndef CAMERA_H
#define CAMERA_H

#include "MouseControllable.h"
#include "SceneObject.h"
#include "MoveCommand.h"

class Camera
: public SceneObject
, public Moveable
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

   // view interface
   sf::Vector2f get_size();
   void set_size(const sf::Vector2f& size);

   const sf::Vector2f& get_center();
   void set_center(const sf::Vector2f& center);

   // moveable interface
   virtual void move(const sf::Vector2f& delta);

   const sf::View& view() const; // for internal use only...

   // viewport management
   void set_viewport(const sf::FloatRect& viewport);
   const sf::FloatRect& get_viewport();

   // mouse control interface
   virtual void drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButton button, ButtonState state, sf::Vector2f pos);

   // scene graph interface
   virtual sf::Vector2f get_world_coordinate(const sf::Vector2f& point); // affected by camera transform
   virtual sf::Vector2f get_screen_coordinate(const sf::Vector2f& point);

   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);
   virtual bool intersects(const SceneObject& other);

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

private:
   std::string id_;
   float zoom_factor_;

   sf::Vector2f original_center_;
   sf::View* view_;

protected:
   // scene graph interface hooks
   virtual void do_apply_transform(RenderSurface& surface, sf::RenderStates& render_states);
};
#endif
