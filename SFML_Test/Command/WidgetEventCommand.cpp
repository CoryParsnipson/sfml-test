#include "WidgetEventCommand.h"
#include "GetWidgetCommand.h"
#include "Gamepad.h"

WidgetEventCommand::WidgetEventCommand(WidgetOp action, Scene::SceneGraph* scene_graph, Gamepad* gamepad)
: Command("WidgetEventCommand")
, action_(action)
, target_(new GetWidgetCommand(scene_graph))
, gamepad_(gamepad)
{
}

WidgetEventCommand::~WidgetEventCommand() {
   delete this->target_;
}

void WidgetEventCommand::execute() {
   sf::Vector2f cur_pos = this->gamepad_->cursor_position();
   sf::Vector2f prev_cur_pos = this->gamepad_->prev_cursor_position();

   Widget* w = nullptr;
   Widget* w_prev = nullptr;

   switch (this->action_) {
   case WidgetOp::MouseMove:
      this->target_->target(cur_pos);
      this->target_->execute();

      if (this->target_->get().size() > 0) {
         w = this->target_->get()[0];
      }
      
      this->target_->target(prev_cur_pos);
      this->target_->execute();

      if (this->target_->get().size() > 0) {
         w_prev = this->target_->get()[0];
      }

      if (w != w_prev) {
         if (w) {
            w->on_mouse_in();
         }

         if (w_prev) {
            w_prev->on_mouse_out();
         }
      }
   break;
   case WidgetOp::MouseClick:
      this->target_->target(cur_pos);
      this->target_->execute();

      if (this->target_->get().size() > 0) {
         this->target_->get()[0]->on_click();
      }
   break;
   case WidgetOp::MouseRelease:
      this->target_->target(cur_pos);
      this->target_->execute();

      if (this->target_->get().size() > 0) {
         this->target_->get()[0]->on_release();
      }
   break;
   }
}

void WidgetEventCommand::unexecute() {
   // empty
}
