#include "SwitchSceneCommand.h"
#include "Scene.h"

SwitchSceneCommand::SwitchSceneCommand(Scene* curr_scene, Scene* next_scene /* = nullptr */)
: curr_scene_(curr_scene)
, next_scene_(next_scene)
{
}

SwitchSceneCommand::~SwitchSceneCommand() {
}

void SwitchSceneCommand::execute() {
   if (!this->curr_scene_ || !this->next_scene_) {
      return;
   }

   this->curr_scene_->switch_scene(this->next_scene_);
}

void SwitchSceneCommand::unexecute() {
   if (!this->curr_scene_ || !this->next_scene_) {
      return;
   }

   this->next_scene_->switch_scene(this->curr_scene_);
}
