#include "Command.h"

Command::Command(std::string id /* = "Command" */)
: id_(std::move(id))
{}

Command::~Command() {
}

Command::operator std::string() const {
   return this->to_string();
}

std::string Command::to_string() const {
   return "[Command: " + this->id_ + "]";
}

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const Command& command) {
   stream << command.to_string();
   return stream;
}

