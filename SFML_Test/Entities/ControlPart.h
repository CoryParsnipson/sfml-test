#ifndef CONTROL_PART_H
#define CONTROL_PART_H

#include "Part.h"
#include "InputListener.h"

// forward declarations
class Game;

class ControlPart
: public Part
, public InputListener
{
public:
   ControlPart(std::string id = "control") : Part(id) {};
   virtual ~ControlPart() {};

   virtual void process(CloseCommand& c) = 0;
   virtual void process(KeyPressCommand& c) = 0;
   virtual void process(WindowResizeCommand& c) = 0;
   virtual void process(MouseMoveCommand& c) = 0;
   virtual void process(MouseButtonCommand& c) = 0;
   virtual void process(MouseWheelCommand& c) = 0;

   virtual void update(Entity& entity, Game& game) = 0;

protected:
};

#endif
