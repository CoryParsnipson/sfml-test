#ifndef CLICK_COMMAND_H
#define CLICK_COMMAND_H

#include "Command.h"
#include "GetWidgetCommand.h"

class ClickCommand
: public Command
{
public:
   ClickCommand(GetWidgetCommand* target = nullptr);
   
   void target(GetWidgetCommand* target);
   GetWidgetCommand* target();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   GetWidgetCommand* target_;
};

#endif
