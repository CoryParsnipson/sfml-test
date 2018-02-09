#include "CameraResizePolicy.h"
#include "Camera.h"

void CameraResizePolicy::set_camera_size(Camera& camera, const sf::Vector2f& size) {
   camera.resize(size);
}
