#include "CameraSystem.h"

#include "PlayerProfile.h"
#include "CameraAnchor.h"

CameraSystem::CameraSystem(const std::string& id, CameraPtr camera)
: System(id)
, camera_(camera)
{
}

CameraSystem::~CameraSystem() {
}

CameraPtr CameraSystem::camera() const {
   return this->camera_;
}

void CameraSystem::on_init(Game& game) {
   this->subscribe_to().all_of<PlayerProfile, CameraAnchor>();
}

void CameraSystem::on_update(Game& game, Entity& e) {
   this->camera_->zoom(1 / 2.f); // scale everything by 2x
   this->camera_->center(e.space()->position());
}
