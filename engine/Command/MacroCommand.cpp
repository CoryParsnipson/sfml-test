#include "MacroCommand.h"
#include "Service.h"

MacroCommand::MacroCommand(std::string id /* = "MacroCommand" */) {
}

void MacroCommand::add(Command* command) {
   if (!command) {
      return;
   }
   this->commands_.push_back(command);
}

void MacroCommand::execute() {
   Service::get_logger().msg(this->id_, Logger::INFO, "Executing macro command.");

   CommandList::const_iterator it;
   for (it = this->commands_.begin(); it != this->commands_.end(); ++it) {
      (*it)->execute();
   }
}

void MacroCommand::unexecute() {
   CommandList::const_reverse_iterator it;
   for (it = this->commands_.rbegin(); it != this->commands_.rend(); ++it) {
      (*it)->unexecute();
   }
}
