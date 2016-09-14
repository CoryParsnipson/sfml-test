#include "FitCameraResizePolicy.h"
#include "Camera.h"

FitCameraResizePolicy::FitCameraResizePolicy()
{
}

void FitCameraResizePolicy::resize(Camera& camera, sf::Vector2f size) {
   camera.set_size(size);
   camera.set_center(size / 2.f);
}
