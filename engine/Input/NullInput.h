#ifndef NULL_INPUT_H
#define NULL_INPUT_H

#include "Input.h"

class NullInput : public Input {
public:
   NullInput(std::string id = "NullInput")
   : Input(id)
   {}

   virtual void poll_event(Game& game) {}
};

#endif
