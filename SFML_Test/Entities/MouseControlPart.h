#ifndef MOUSE_CONTROL_PART_H
#define MOUSE_CONTROL_PART_H

#include "ControlPart.h"

class MouseControlPart : public ControlPart {
public:
   MouseControlPart(std::string id = "control");
   virtual ~MouseControlPart();

   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);

   virtual void update(Entity2& entity, Viewport& viewport);

protected:
};

#endif
