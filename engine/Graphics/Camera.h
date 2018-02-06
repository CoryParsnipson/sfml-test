#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <string>

#include "InputManager.h"
#include "CameraResizePolicy.h"

// ----------------------------------------------------------------------------
// typedefs
// ----------------------------------------------------------------------------
typedef std::shared_ptr<Camera> CameraPtr;

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class RenderSurface;

// ----------------------------------------------------------------------------
// Camera
//
// This class lets the user manipulate the viewport and view into the game
// graphics.
// ----------------------------------------------------------------------------
class Camera {
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;
   
   Camera(const std::string& id);
   Camera(const std::string& id, const sf::Vector2f& size);
   Camera(const Camera& other);
   virtual ~Camera();

   Camera& operator=(const Camera& other);
   void swap(Camera& other);

   const std::string& id() const;
   void id(std::string id);
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

   void move(const sf::Vector2f& delta);

   sf::FloatRect bounds() const;

   // viewport management
   void set_viewport(const sf::FloatRect& viewport);
   const sf::FloatRect& get_viewport();

   sf::Vector2f get_world_coordinate(const sf::Vector2f& point);
   sf::Vector2f get_screen_coordinate(const sf::Vector2f& point);

   // drawable interface
   virtual void draw(RenderSurface& surface);

   const sf::View& view() const; // for internal use only...

private:
   // TODO: replace all this shit with sf::Transformable
   std::string id_;
   float zoom_factor_;

   sf::Vector2f original_center_;
   sf::View* view_;

   std::shared_ptr<CameraResizePolicy> resize_policy_;

   friend std::ostream& operator<<(std::ostream& stream, const Camera& camera);

   sf::IntRect viewport();
};
#endif
