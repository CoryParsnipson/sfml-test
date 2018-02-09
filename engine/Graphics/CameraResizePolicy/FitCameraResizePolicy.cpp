#include "FitCameraResizePolicy.h"
#include "Camera.h"
#include "Game.h"

FitCameraResizePolicy::FitCameraResizePolicy()
{
}

void FitCameraResizePolicy::resize(Camera& camera, const sf::Vector2f& size) {
   sf::Vector2f effective_size(size.x * camera.viewport().width, size.y * camera.viewport().height);

  Game::logger().msg("FitCameraResizePolicy", Logger::INFO, "Resizing from "
     + std::to_string((int)camera.size().x) + "x" + std::to_string((int)camera.size().y)
     + " to " + std::to_string((int)effective_size.x) + "x" + std::to_string((int)effective_size.y));

   this->set_camera_size(camera, effective_size);
   camera.center(effective_size / 2.f);
}
