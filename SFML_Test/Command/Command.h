#ifndef COMMAND_H
#define COMMAND_H

#include "InputListener.h"

class Command {
public:
   virtual ~Command() {}
   virtual void execute(InputListener& listener) = 0;
};
#endif
