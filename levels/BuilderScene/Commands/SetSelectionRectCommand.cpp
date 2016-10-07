#include "SetSelectionRectCommand.h"
#include "Gamepad.h"

SetSelectionRectCommand::SetSelectionRectCommand(const std::shared_ptr<UpdateSelectionRectCommand>& usr_command, Gamepad* gamepad, Entity* selection_rect, bool modify_click_pos /* = true */, bool set_visible /* = false */, bool unset_visible /* = false */)
: modify_click_pos_(modify_click_pos)
, set_visible_(set_visible)
, unset_visible_(unset_visible)
, gamepad_(gamepad)
, usr_command_(usr_command)
{
}

SetSelectionRectCommand::~SetSelectionRectCommand() {
}

void SetSelectionRectCommand::execute() {
   if (!this->gamepad_) {
      return;
   }

   if (this->modify_click_pos_) {
      this->usr_command_->click_pos(this->gamepad_->cursor_position());
      this->usr_command_->release_pos(this->gamepad_->cursor_position());
   } else {
      this->usr_command_->release_pos(this->gamepad_->cursor_position());
   }
   
   if (this->set_visible_) {
      this->usr_command_->visible(true);
   } else if (this->unset_visible_) {
      this->usr_command_->visible(false);
   }

   this->usr_command_->execute();
}

void SetSelectionRectCommand::unexecute() {
   // implement me
}
