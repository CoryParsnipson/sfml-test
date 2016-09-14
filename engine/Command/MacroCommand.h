#ifndef MACRO_COMMAND_H
#define MACRO_COMMAND_H

#include "sfml.h"

#include "Command.h"

// ----------------------------------------------------------------------------
// MacroCommand base class
//
// The macro command class is a command that contains multiple other commands
// (including other macro commands) that it executes in order.
// ----------------------------------------------------------------------------
class MacroCommand
: public Command
{
public:
   MacroCommand(std::string id = "MacroCommand");

   void add(Command* command);

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   CommandList commands_;
};

#endif
