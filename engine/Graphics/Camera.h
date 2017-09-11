#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <string>

#include "Input.h"
#include "SceneObject.h"
#include "CameraResizePolicy.h"

class Camera
: public SceneObject
, public Moveable
{
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;
   
   Camera(const std::string& id);
   Camera(const std::string& id, const sf::Vector2f& size);
   virtual ~Camera();

   const std::string& id() const;
   virtual std::string to_string() const;

   void reset_pan();
   void reset_zoom();

   sf::Vector2f get_pan_delta();

   void policy(std::shared_ptr<CameraResizePolicy> policy);
   const std::shared_ptr<CameraResizePolicy>& policy();

   void resize(sf::Vector2f size);

   // view interface
   sf::Vector2f get_size() const;
   void set_size(const sf::Vector2f& size);

   float get_scale();
   void set_scale(float factor);

   const sf::Vector2f& get_center() const;
   void set_center(const sf::Vector2f& center);

   sf::FloatRect bounds() const;

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

   // serializable interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   std::string id_;
   float zoom_factor_;

   sf::Vector2f original_center_;
   sf::View* view_;

   std::shared_ptr<CameraResizePolicy> resize_policy_;

   friend std::ostream& operator<<(std::ostream& stream, const Camera& camera);

   sf::IntRect viewport();

protected:
   // scene graph interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states);
};
#endif
