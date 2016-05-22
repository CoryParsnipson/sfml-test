#ifndef CONTROL_PART_H
#define CONTROL_PART_H

#include "Part.h"
#include "InputListener.h"

class ControlPart
: public Part
, public InputListener
{
public:
   ControlPart(std::string id = "control")
   : Part(id)
   , InputListener(id)
   {};

   virtual ~ControlPart() {};

   // input event processing
   virtual void process(CloseInputEvent& e) = 0;
   virtual void process(ResizeInputEvent& e) = 0;
   virtual void process(KeyPressInputEvent& e) = 0;
   virtual void process(MouseMoveInputEvent& e) = 0;
   virtual void process(MouseWheelInputEvent& e) = 0;
   virtual void process(MouseButtonInputEvent& e) = 0;

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

   // serialize interface
   virtual Serialize::SerialObj serialize() = 0;
   virtual void deserialize(Serialize::SerialObj& obj) = 0;

protected:
};

#endif
