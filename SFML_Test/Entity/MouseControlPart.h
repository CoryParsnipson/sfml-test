#ifndef MOUSE_CONTROL_PART_H
#define MOUSE_CONTROL_PART_H

#include "ControlPart.h"
#include "MouseControllable.h"

#include "CloseInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

class MouseControlPart : public ControlPart {
public:
   const float MOUSE_PAN_COEFFICIENT = 1.f;
   const float WINDOW_RESIZE_COEFFICIENT = 10.f;

   MouseControlPart(std::string id = "control");
   virtual ~MouseControlPart();

   // mouse util interface
   virtual void set_controllable(MouseControllable* c = nullptr);

   // input event processing
   virtual void process(CloseInputEvent& e);
   virtual void process(ResizeInputEvent& e);
   virtual void process(KeyPressInputEvent& e);
   virtual void process(MouseMoveInputEvent& e);
   virtual void process(MouseWheelInputEvent& e);
   virtual void process(MouseButtonInputEvent& e);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   // serialize interface
   virtual Serialize::SerialObj serialize();
   virtual void deserialize(Serialize::SerialObj& obj);

protected:
   bool is_panning;
   int zoom_delta;

   MouseControllable* controllable_;

   MouseButton button_;
   sf::Vector2f panning_anchor;
   sf::Vector2f last_mouse_pos;
};

#endif
