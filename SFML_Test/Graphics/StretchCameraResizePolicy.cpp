#include "StretchCameraResizePolicy.h"
#include "Camera.h"

StretchCameraResizePolicy::StretchCameraResizePolicy(float x, float y)
: size_(x, y)
{
}

StretchCameraResizePolicy::StretchCameraResizePolicy(sf::Vector2f size)
: size_(std::move(size))
{
}

void StretchCameraResizePolicy::resize(Camera& camera) {
   camera.set_size(this->size_);
   camera.set_center(0.5f * this->size_);
}
