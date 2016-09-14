#include "ToggleVisibleCommand.h"
#include "SceneObject.h"

ToggleVisibleCommand::ToggleVisibleCommand(SceneObject* scene_object)
: scene_object_(scene_object)
{
}

ToggleVisibleCommand::~ToggleVisibleCommand() {
}

void ToggleVisibleCommand::execute() {
   if (!this->scene_object_) {
      return;
   }

   this->scene_object_->visible(!this->scene_object_->visible());
}

void ToggleVisibleCommand::unexecute() {
   this->execute();
}
