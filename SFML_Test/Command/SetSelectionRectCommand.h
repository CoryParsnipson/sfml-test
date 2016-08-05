#ifndef SET_SELECTION_RECT_COMMAND
#define SET_SELECTION_RECT_COMMAND

#include "dependencies.h"
#include "Command.h"
#include "UpdateSelectionRectCommand.h"

// ----------------------------------------------------------------------------
// forward declaration
// ----------------------------------------------------------------------------
class Gamepad;

// ----------------------------------------------------------------------------
// SetSelectionRectCommand
//
// Given a gamepad and a selection rectangle entity, update the selection
// rectangle
// ----------------------------------------------------------------------------
class SetSelectionRectCommand : public Command {
public:
   SetSelectionRectCommand(const std::shared_ptr<UpdateSelectionRectCommand>& usr_command, Gamepad* gamepad, Entity* selection_rect, bool modify_click_pos = true, bool set_visible = false, bool unset_visible = false);
   virtual ~SetSelectionRectCommand();

   virtual void execute();
   virtual void unexecute();

private:
   bool modify_click_pos_;
   bool set_visible_;
   bool unset_visible_;
   Gamepad* gamepad_;
   
   std::shared_ptr<UpdateSelectionRectCommand> usr_command_;
};

#endif
