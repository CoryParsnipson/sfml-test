#include "LeftMouseReleaseCommand.h"
#include "SceneObject.h"
#include "Gamepad.h"

LeftMouseReleaseCommand::LeftMouseReleaseCommand(Gamepad* g, SceneObject* scene_graph)
: gamepad_(g)
, target_(scene_graph)
{
}

LeftMouseReleaseCommand::~LeftMouseReleaseCommand() {
}

void LeftMouseReleaseCommand::execute() {
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
      (*it)->on_left_release();
      break;
   }
}

void LeftMouseReleaseCommand::unexecute() {
   // TODO: implement me
}
