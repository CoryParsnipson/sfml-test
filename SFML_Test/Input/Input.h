#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>

#include "Observer.h"

class Game;
class InputEvent;

class Input
: public Subject<InputEvent>
{
public:
   Input(std::string id = "Input");

   virtual void poll_event(Game& game);

protected:
};

#endif
