#include "MoveCameraCommand.h"
#include "Camera.h"

MoveCameraCommand::MoveCameraCommand(Camera& camera, sf::Vector2f delta /* = sf::Vector2f(0, 0) */, std::string id /* = "MoveCameraCommand" */)
: Command(id)
, delta_(std::move(delta))
, camera_(&camera)
{}

MoveCameraCommand::~MoveCameraCommand() {
}

void MoveCameraCommand::execute() {
   this->camera_->move(this->delta_);
}

void MoveCameraCommand::unexecute() {
   this->camera_->move(-1.f * this->delta_);
}

void MoveCameraCommand::set_delta(sf::Vector2f delta) {
   this->delta_ = delta;
}

sf::Vector2f MoveCameraCommand::get_delta() {
   return this->delta_;
}
