#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>

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

#endif
