#include "RightMouseClickCommand.h"
#include "SceneObject.h"
#include "Gamepad.h"

RightMouseClickCommand::RightMouseClickCommand(Gamepad* g, SceneObject* scene_graph)
: gamepad_(g)
, target_(scene_graph)
{
}

RightMouseClickCommand::~RightMouseClickCommand() {
}

void RightMouseClickCommand::execute() {
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
      (*it)->on_right_click();
      break;
   }
}

void RightMouseClickCommand::unexecute() {
   // TODO: implement me
}

