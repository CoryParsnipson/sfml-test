#ifndef MOUSE_CONTROL_PART_H
#define MOUSE_CONTROL_PART_H

#include "ControlPart.h"
#include "MouseUtil.h"
#include "MouseButtonCommand.h"

class MouseControlPart : public ControlPart {
public:
   const float MOUSE_PAN_COEFFICIENT = 1.f;
   const float WINDOW_RESIZE_COEFFICIENT = 10.f;

   MouseControlPart(std::string id = "control");
   virtual ~MouseControlPart();

   // mouse util interface
   virtual void set_controllable(MouseControllable* c = nullptr);

   // command interface
   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);
   
   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   bool is_panning;
   int zoom_delta;
   
   MouseControllable* controllable_;

   MouseButtonCommand* mouse_buttons_;
   sf::Vector2f panning_anchor;
   sf::Vector2f last_mouse_pos;
};

#endif
