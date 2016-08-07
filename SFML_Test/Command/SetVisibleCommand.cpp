#include "SetVisibleCommand.h"
#include "SceneObject.h"

SetVisibleCommand::SetVisibleCommand(SceneObject* scene_object, bool set_visible)
: new_visible_state_(set_visible)
, old_visible_state_(scene_object ? scene_object->visible() : false)
, scene_object_(scene_object)
{
}

SetVisibleCommand::~SetVisibleCommand() {
}

void SetVisibleCommand::execute() {
   if (!this->scene_object_) {
      return;
   }

   this->old_visible_state_ = this->scene_object_->visible();
   this->scene_object_->visible(this->new_visible_state_);
}

void SetVisibleCommand::unexecute() {
   if (!this->scene_object_) {
      return;
   }

   this->scene_object_->visible(this->old_visible_state_);
}
