#ifndef FIT_CAMERA_RESIZE_POLICY_H
#define FIT_CAMERA_RESIZE_POLICY_H

#include "CameraResizePolicy.h"

// ----------------------------------------------------------------------------
// FitCameraResizePolicy
//
// This policy sets the camera to the provided size vector on resize. This will
// show more of the underlying game world instead of stretch and/or resizing the
// contents.
// ----------------------------------------------------------------------------
class FitCameraResizePolicy : public CameraResizePolicy {
public:
   FitCameraResizePolicy();

   virtual void resize(Camera& camera, const sf::Vector2f& size);
};

#endif
