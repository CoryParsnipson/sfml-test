#ifndef MOUSE_UTIL_H
#define MOUSE_UTIL_H

#include "MouseButtonCommand.h"

// abstract interface to define actions that mouse control options
class MouseControllable {
public:
   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta) = 0;

   virtual float get_scale() = 0;
   virtual void set_scale(float factor) = 0;

   virtual void click(MouseButtonCommand& c) = 0;
};

#endif
