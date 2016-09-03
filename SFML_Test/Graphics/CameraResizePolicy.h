#ifndef CAMERA_RESIZE_POLICY_H
#define CAMERA_RESIZE_POLICY_H

#include "dependencies.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;

// ----------------------------------------------------------------------------
// CameraResizePolicy
//
// Specifies behavior of camera on resize event.
// ----------------------------------------------------------------------------
class CameraResizePolicy {
public:
   virtual void resize(Camera& camera, sf::Vector2f size) = 0;
};

#endif
