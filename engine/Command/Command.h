#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>
#include <vector>

// ----------------------------------------------------------------------------
// forward declarations 
// ----------------------------------------------------------------------------
class Command;

// ----------------------------------------------------------------------------
// aliases
// ----------------------------------------------------------------------------
using CommandList = std::vector<Command*>;

// ----------------------------------------------------------------------------
// Command abstract base
// 
// This class encapsulates a request packet that can be passed from module
// to module in order to execute functions when certain conditions are met.
// ----------------------------------------------------------------------------
class Command {
public:
   Command(std::string id = "Command");
   virtual ~Command();

   virtual void execute() = 0;
   virtual void unexecute() = 0;

   operator std::string() const;

private:
   friend std::ostream& operator<<(std::ostream& stream, const Command& command);
   std::string to_string() const;

protected:
   std::string id_;
};

// ----------------------------------------------------------------------------
// NullCommand
// 
// This is a command that does not execute anything. This should be used in
// place of nullptr when expecting a Command pointer.
// ----------------------------------------------------------------------------
class NullCommand : public Command {
public:
   NullCommand(std::string id = "NullCommand") : Command(id) {}
   virtual ~NullCommand() {}

   virtual void execute() {}
   virtual void unexecute() {}
};

#endif
