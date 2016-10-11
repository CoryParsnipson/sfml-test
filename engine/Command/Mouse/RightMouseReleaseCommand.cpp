#include "RightMouseReleaseCommand.h"
#include "SceneObject.h"
#include "Gamepad.h"

RightMouseReleaseCommand::RightMouseReleaseCommand(Gamepad* g, SceneObject* scene_graph)
: gamepad_(g)
, target_(scene_graph)
{
}

RightMouseReleaseCommand::~RightMouseReleaseCommand() {
}

void RightMouseReleaseCommand::execute() {
   if (!this->gamepad_) {
      return;
   }

   this->target_.target(this->gamepad_->cursor_position());
   this->target_.execute();

   const SceneObjectList& objects = this->target_.get();
   for (SceneObjectList::const_iterator it = objects.begin(); it != objects.end(); ++it) {
      if (!(*it)->clickable()) {
         continue;
      }
      (*it)->on_right_release();
      break;
   }
}

void RightMouseReleaseCommand::unexecute() {
   // TODO: implement me
}
