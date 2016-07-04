#ifndef HOVER_COMMND_H
#define HOVER_COMMAND_H

#include "Command.h"
#include "GetWidgetCommand.h"

class HoverCommand
: public Command
{
public:
   HoverCommand(GetWidgetCommand* target = nullptr);
   
   void target(GetWidgetCommand* target);
   GetWidgetCommand* target();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   GetWidgetCommand* target_;
};

#endif
