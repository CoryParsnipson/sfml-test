#ifndef MOUSE_CONTROL_PART_H
#define MOUSE_CONTROL_PART_H

#include "ControlPart.h"

class Game;
class Viewport;

class MouseControlPart : public ControlPart {
public:
   const float MOUSE_PAN_COEFFICIENT = 1.f;
   const float WINDOW_RESIZE_COEFFICIENT = 10.f;

   MouseControlPart(std::string id = "control", Viewport* viewport = nullptr);
   virtual ~MouseControlPart();

   virtual void process(CloseCommand& c);
   virtual void process(KeyPressCommand& c);
   virtual void process(WindowResizeCommand& c);
   virtual void process(MouseMoveCommand& c);
   virtual void process(MouseButtonCommand& c);
   virtual void process(MouseWheelCommand& c);

   virtual void update(Entity& entity, Game& game);

protected:
   bool is_panning;
   int zoom_delta;

   Viewport* viewport_;

   sf::Vector2f panning_anchor;
   sf::Vector2f last_mouse_pos;
   sf::Vector2f* last_click_pos;
};

#endif
