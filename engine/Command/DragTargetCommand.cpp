#include "DragTargetCommand.h"
#include "DragCommand.h"

DragTargetCommand::DragTargetCommand(DragCommand* command, Moveable* moveable /* = nullptr */)
: moveable_(moveable)
, drag_command_(command)
{
}

DragTargetCommand::~DragTargetCommand() {
}

void DragTargetCommand::execute() {
   this->old_moveable_ = this->drag_command_->moveable();
   this->drag_command_->moveable(this->moveable_);
}

void DragTargetCommand::unexecute() {
   this->drag_command_->moveable(this->old_moveable_);
}

void DragTargetCommand::moveable(Moveable* moveable) {
   this->moveable_ = moveable;
}
