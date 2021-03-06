#ifndef STRETCH_CAMERA_RESIZE_POLICY_H
#define STRETCH_CAMERA_RESIZE_POLICY_H

#include <SFML/System/Vector2.hpp>

#include "CameraResizePolicy.h"

// ----------------------------------------------------------------------------
// StretchCameraResizePolicy
//
// This policy sets the camera to a fixed size specified during policy's
// constructor. Any resize event will cause this camera to stretch to fit the
// new window size.
// ----------------------------------------------------------------------------
class StretchCameraResizePolicy : public CameraResizePolicy {
public:
   StretchCameraResizePolicy(float x, float y);
   StretchCameraResizePolicy(sf::Vector2f size);

   virtual void resize(Camera& camera, const sf::Vector2f& size);

private:
   sf::Vector2f size_;
};

#endif
