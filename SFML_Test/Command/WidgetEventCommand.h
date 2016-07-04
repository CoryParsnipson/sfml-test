#ifndef WIDGET_EVENT_COMMAND_H
#define WIDGET_EVENT_COMMAND_H

#include "Command.h"
#include "Widget.h"
#include "GetWidgetCommand.h"

using WidgetFunction = void (Widget::*)();

class WidgetEventCommand : public Command {
public:
   WidgetEventCommand(WidgetFunction action, GetWidgetCommand* target = nullptr);

   void target(GetWidgetCommand* target);
   GetWidgetCommand* target();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   WidgetFunction action_;
   GetWidgetCommand* target_;
};

#endif
