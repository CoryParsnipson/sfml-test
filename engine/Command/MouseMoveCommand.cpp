#include <algorithm>

#include "MouseMoveCommand.h"
#include "SceneObject.h"
#include "Gamepad.h"

MouseMoveCommand::MouseMoveCommand(Gamepad* g, SceneObject* scene_graph)
: gamepad_(g)
, target_(scene_graph)
{
}

MouseMoveCommand::~MouseMoveCommand() {
}

void MouseMoveCommand::execute() {
   if (!this->gamepad_) {
      return;
   }

   sf::Vector2f pos(this->gamepad_->cursor_position());
   sf::Vector2f prev_pos(this->gamepad_->prev_cursor_position());

   this->target_.target(pos);
   this->target_.execute();

   SceneObjectList curr_list = this->target_.get();

   this->target_.target(prev_pos);
   this->target_.execute();

   SceneObjectList prev_list = this->target_.get();

   SceneObjectList::const_iterator it;
   for (it = curr_list.begin(); it != curr_list.end(); ++it) {
      if (std::find(prev_list.begin(), prev_list.end(), *it) != prev_list.end()) {
         // intersection of curr and prev lists should execute mouse move command
         (*it)->on_mouse_move();
      } else {
         // difference of curr - prev should get execute mouse in command
         (*it)->on_mouse_in();
      }
   }

   for (it = prev_list.begin(); it != prev_list.end(); ++it) {
      if (std::find(curr_list.begin(), curr_list.end(), *it) == curr_list.end()) {
         // difference of prev - curr should get execute mouse out command
         (*it)->on_mouse_out();
      }
   }
}

void MouseMoveCommand::unexecute() {
   // TODO: implement me
}

