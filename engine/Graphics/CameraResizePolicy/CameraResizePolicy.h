#ifndef CAMERA_RESIZE_POLICY_H
#define CAMERA_RESIZE_POLICY_H

#include <memory>
#include <SFML/System/Vector2.hpp>

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class CameraResizePolicy;

// ----------------------------------------------------------------------------
// typedefs
// ----------------------------------------------------------------------------
typedef std::shared_ptr<CameraResizePolicy> CameraResizePolicyPtr;

// ----------------------------------------------------------------------------
// CameraResizePolicy
//
// Specifies behavior of camera on resize event.
// ----------------------------------------------------------------------------
class CameraResizePolicy {
public:
   virtual void resize(Camera& camera, const sf::Vector2f& size) = 0;

protected:
   void set_camera_size(Camera& camera, const sf::Vector2f& size);
};

#endif
