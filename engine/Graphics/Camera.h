#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

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
   Camera(const std::string& id);
   Camera(const std::string& id, const sf::Vector2f& size);
   Camera(const Camera& other);
   virtual ~Camera();

   Camera& operator=(const Camera& other);
   void swap(Camera& other);

   const std::string& id() const;
   void id(std::string id);

   virtual std::string to_string() const;

   // draw interface
   virtual void draw(RenderSurface& surface);

   void policy(CameraResizePolicyPtr policy);
   CameraResizePolicyPtr policy();

   // transform interface
   const sf::Vector2f& center() const;
   void center(const sf::Vector2f& center);
   void center(float x, float y);
   
   const sf::Vector2f& size() const;
   void size(const sf::Vector2f& size);
   void size(float x, float y);

   float rotation() const;
   void rotation(float angle);

   float zoom() const;
   void zoom(float factor);

   sf::Vector2f position() const;
   void position(const sf::Vector2f& position);
   void position(float x, float y);

   void move(const sf::Vector2f& delta);
   void move(float x, float y);

   sf::FloatRect bounds() const;

   const sf::Transform& transform() const;
   const sf::Transform& inverse_transform() const;

   void viewport(const sf::FloatRect& viewport);
   const sf::FloatRect& viewport();

   sf::Vector2f get_world_coordinate(const sf::Vector2f& point);
   sf::Vector2f get_screen_coordinate(const sf::Vector2f& point);

protected:
   sf::IntRect absolute_viewport();

private:
   friend class RenderSurface;
   friend class CameraResizePolicy;

   std::string id_;
   sf::View view_;
   sf::Transformable transform_;
   std::shared_ptr<CameraResizePolicy> resize_policy_;

   void resize(const sf::Vector2f& size);

   friend std::ostream& operator<<(std::ostream& stream, const Camera& camera);
};
#endif
