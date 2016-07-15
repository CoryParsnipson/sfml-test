#ifndef WIDGET_EVENT_COMMAND_H
#define WIDGET_EVENT_COMMAND_H

#include "Command.h"
#include "Widget.h"
#include "Scene.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;
class GetWidgetCommand;

// ----------------------------------------------------------------------------
// enum classes
// ----------------------------------------------------------------------------
enum class WidgetOp {
   MouseMove,
   MouseClick,
   MouseRelease
};

// ----------------------------------------------------------------------------
// WidgetEventCommand
//
// This command allows the caller to interact with widgets.
// ----------------------------------------------------------------------------
class WidgetEventCommand : public Command {
public:
   WidgetEventCommand(WidgetOp action, SceneGraphNode* scene_graph, Gamepad* gamepad);
   virtual ~WidgetEventCommand();

   // command interface
   virtual void execute();
   virtual void unexecute();

protected:
   WidgetOp action_;
   GetWidgetCommand* target_;
   Gamepad* gamepad_;
};

#endif
