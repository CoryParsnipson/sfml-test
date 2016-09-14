#include "ZoomCommand.h"
#include "Gamepad.h"
#include "Camera.h"
#include "Grid.h"

ZoomCommand::ZoomCommand(Camera* camera, Grid* grid, Gamepad* gamepad)
: old_scale_factor_(1.f)
, camera_(camera)
, grid_(grid)
, gamepad_(gamepad)
{
}

ZoomCommand::~ZoomCommand() {
}

void ZoomCommand::execute() {
   if (!this->camera_ || !this->gamepad_) {
      return;
   }

   this->old_scale_factor_ = this->camera_->get_scale();

   this->camera_->set_scale(this->camera_->get_scale() - this->gamepad_->wheel_delta() / this->ZOOM_COEFFICIENT);
   this->grid_->set_scale(this->camera_->get_scale());
}

void ZoomCommand::unexecute() {
   if (!this->camera_ || !this->gamepad_) {
      return;
   }

   this->camera_->set_scale(this->old_scale_factor_);
   this->grid_->set_scale(this->old_scale_factor_);
}
