#include "ClickCommand.h"

ClickCommand::ClickCommand(GetWidgetCommand* target /* = nullptr */)
: Command("ClickCommand")
, target_(target)
{
}

void ClickCommand::target(GetWidgetCommand* target) {
   this->target_ = target;
}

GetWidgetCommand* ClickCommand::target() {
   return this->target_;
}

void ClickCommand::execute() {
   if (this->target_ && this->target_->get().size() > 0) {
      this->target_->get()[0]->on_click();
   }
}

void ClickCommand::unexecute() {
   // empty
}
