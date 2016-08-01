#include "DragCommand.h"
#include "MoveCommand.h"
#include "Gamepad.h"

DragCommand::DragCommand(Gamepad* gamepad, Moveable* moveable /* = nullptr */)
: Command("DragCommand")
, gamepad_(gamepad)
, move_command_(moveable)
{
}

DragCommand::~DragCommand() {
}

void DragCommand::execute() {
   if (!this->gamepad_) {
      return;
   }

   this->move_command_.delta(this->gamepad_->prev_cursor_position() - this->gamepad_->cursor_position());
   this->move_command_.execute();
}

void DragCommand::unexecute() {
   this->move_command_.unexecute();
}

void DragCommand::moveable(Moveable* moveable) {
   this->move_command_.moveable(moveable);
}

Moveable* DragCommand::moveable() {
   return this->move_command_.moveable();
}
