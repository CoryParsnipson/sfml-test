#ifndef NULL_INPUT_CONTROLLER_H
#define NULL_INPUT_CONTROLLER_H

#include "InputController.h"

class NullInputController : public InputController {
public:
   NullInputController() {};
   virtual ~NullInputController() {};

   virtual void registerInputListener(InputListener* listener) {};
   virtual void pollEvents(Game& game) {};
};

#endif
