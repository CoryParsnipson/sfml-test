#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <vector>

#include "InputListener.h"

// forward declarations
class Game;

// this class is an interface between SFML and the proprietary
// input handler class and command hierarchy
class InputController {
public:
   typedef std::vector<InputListener*> InputListenerList;

   InputController();

   virtual void registerInputListener(InputListener* listener);
   virtual void unregisterInputListener(InputListener* listener);
   virtual void pollEvents(Game& game);

private:
   InputListenerList listeners_;
};

#endif
