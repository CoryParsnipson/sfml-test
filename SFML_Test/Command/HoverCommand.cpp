#include "HoverCommand.h"

HoverCommand::HoverCommand(GetWidgetCommand* target /* = nullptr */)
: Command("HoverCommand")
, target_(target)
{
}

void HoverCommand::target(GetWidgetCommand* target) {
   this->target_ = target;
}

GetWidgetCommand* HoverCommand::target() {
   return this->target_;
}

void HoverCommand::execute() {
   if (this->target_ && this->target_->get().size() > 0) {
      this->target_->get()[0]->on_hover();
   }
}

void HoverCommand::unexecute() {
   // empty
}
