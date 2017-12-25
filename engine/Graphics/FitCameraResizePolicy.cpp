#include "FitCameraResizePolicy.h"
#include "Camera.h"
#include "Game.h"

FitCameraResizePolicy::FitCameraResizePolicy()
{
}

void FitCameraResizePolicy::resize(Camera& camera, sf::Vector2f size) {
   // account for viewport settings
   size.x *= camera.get_viewport().width;
   size.y *= camera.get_viewport().height;

  Game::logger().msg("FitCameraResizePolicy", Logger::INFO, "Resizing from "
     + std::to_string((int)camera.get_size().x) + "x" + std::to_string((int)camera.get_size().y)
     + " to " + std::to_string((int)size.x) + "x" + std::to_string((int)size.y));

   camera.set_size(size);
   camera.set_center(size / 2.f);
}
