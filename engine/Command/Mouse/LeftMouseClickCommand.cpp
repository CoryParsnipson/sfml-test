#include "LeftMouseClickCommand.h"
#include "SceneObject.h"
#include "Gamepad.h"

LeftMouseClickCommand::LeftMouseClickCommand(Gamepad* g, SceneObject* scene_graph)
: gamepad_(g)
, target_(scene_graph)
{
}

LeftMouseClickCommand::~LeftMouseClickCommand() {
}

void LeftMouseClickCommand::execute() {
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
      (*it)->on_left_click();
      break;
   }
}

void LeftMouseClickCommand::unexecute() {
   // TODO: implement me
}
