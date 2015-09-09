#ifndef COMMAND_H
#define COMMAND_H

#include "../Entities/GameEntity.h"

class Command {
public:
   virtual ~Command() {}
   virtual void execute(GameEntity& entity) = 0;
};
#endif
