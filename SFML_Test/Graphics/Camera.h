#ifndef CAMERA_H
#define CAMERA_H

#include "SceneObject.h"
#include "MoveCommand.h"
#include "CameraResizePolicy.h"

class Camera
: public SceneObject
, public Moveable
{
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;
   
   static const float DEFAULT_WIDTH;
   static const float DEFAULT_HEIGHT;

   Camera(const std::string& id);
   Camera(const std::string& id, const sf::Vector2f& size);
   virtual ~Camera();

   const std::string& id();
   const std::string& to_string();

   void reset_pan();
   void reset_zoom();

   sf::Vector2f get_pan_delta();

   void policy(std::shared_ptr<CameraResizePolicy> policy);
   const std::shared_ptr<CameraResizePolicy>& policy();

   void resize(sf::Vector2f size);

   // view interface
   sf::Vector2f get_size();
   void set_size(const sf::Vector2f& size);

   float get_scale();
   void set_scale(float factor);

   const sf::Vector2f& get_center();
   void set_center(const sf::Vector2f& center);

   // moveable interface
   virtual void move(const sf::Vector2f& delta);

   const sf::View& view() const; // for internal use only...

   // viewport management
   void set_viewport(const sf::FloatRect& viewport);
   const sf::FloatRect& get_viewport();

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

   std::shared_ptr<CameraResizePolicy> resize_policy_;

protected:
   // scene graph interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states);
};
#endif
