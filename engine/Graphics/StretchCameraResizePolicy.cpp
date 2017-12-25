#include "StretchCameraResizePolicy.h"
#include "Camera.h"
#include "Game.h"

StretchCameraResizePolicy::StretchCameraResizePolicy(float x, float y)
: size_(x, y)
{
}

StretchCameraResizePolicy::StretchCameraResizePolicy(sf::Vector2f size)
: size_(std::move(size))
{
}

void StretchCameraResizePolicy::resize(Camera& camera, sf::Vector2f size) {
  Game::logger().msg("StretchCameraResizePolicy", Logger::INFO, "Resizing from "
     + std::to_string((int)camera.get_size().x) + "x" + std::to_string((int)camera.get_size().y)
     + " to " + std::to_string((int)size.x) + "x" + std::to_string((int)size.y));

   camera.set_size(this->size_);
   camera.set_center(0.5f * this->size_);
}
