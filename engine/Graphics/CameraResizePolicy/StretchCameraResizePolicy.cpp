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

void StretchCameraResizePolicy::resize(Camera& camera, const sf::Vector2f& size) {
  Game::logger().msg("StretchCameraResizePolicy", Logger::INFO, "Resizing from "
     + std::to_string((int)camera.size().x) + "x" + std::to_string((int)camera.size().y)
     + " to " + std::to_string((int)this->size_.x) + "x" + std::to_string((int)this->size_.y));

   this->set_camera_size(camera, this->size_);
   camera.center(0.5f * this->size_);
}
