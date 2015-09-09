#ifndef CLOSE_COMMAND_H
#define CLOSE_COMMAND_H

#include "Command.h"

class CloseCommand : public Command {
public:
   virtual void execute(GameEntity& entity) {
      entity.process(*this);
   }
};

#endif
