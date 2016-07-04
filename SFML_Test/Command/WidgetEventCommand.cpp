#include "WidgetEventCommand.h"

WidgetEventCommand::WidgetEventCommand(WidgetFunction action, GetWidgetCommand* target /* = nullptr */)
: Command("WidgetEventCommand")
, action_(action)
, target_(target)
{
}

void WidgetEventCommand::target(GetWidgetCommand* target) {
   this->target_ = target;
}

GetWidgetCommand* WidgetEventCommand::target() {
   return this->target_;
}

void WidgetEventCommand::execute() {
   this->target_->execute();

   if (this->target_ && this->target_->get().size() > 0) {
      (this->target_->get()[0]->*this->action_)();
   }
}

void WidgetEventCommand::unexecute() {
   // empty
}
