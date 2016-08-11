#include "ResetCameraCommand.h"
#include "Camera.h"
#include "Grid.h"

ResetCameraCommand::ResetCameraCommand(Camera* camera, Grid* grid /* = nullptr */)
: camera_(camera)
, grid_(grid)
, scale_factor_(1.f)
, pan_delta_(0, 0)
{
}

ResetCameraCommand::~ResetCameraCommand() {
}

void ResetCameraCommand::execute() {
   if (!this->camera_) {
      return;
   }

   this->scale_factor_ = this->camera_->get_scale();
   this->pan_delta_ = this->camera_->get_pan_delta();

   this->camera_->reset_pan();
   this->camera_->reset_zoom();

   if (this->grid_) {
      this->grid_->set_scale(1.f);
      this->grid_->move(this->pan_delta_);
   }
}

void ResetCameraCommand::unexecute() {
   if (!this->camera_) {
      return;
   }

   this->camera_->move(-this->pan_delta_);
   this->camera_->set_scale(this->scale_factor_);

   if (this->grid_) {
      this->grid_->set_scale(this->scale_factor_);
      this->grid_->move(-this->pan_delta_);
   }
}
