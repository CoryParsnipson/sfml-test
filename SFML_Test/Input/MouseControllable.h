#ifndef MOUSE_CONTROLLABLE_H
#define MOUSE_CONTROLLABLE_H

#include "MouseButtonInputEvent.h"

// ----------------------------------------------------------------------------
// MouseControllable
// 
// This is an interface for things that need to be controlled by mouse input.
// ----------------------------------------------------------------------------
class MouseControllable {
public:
   virtual void drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta) = 0;

   virtual float get_scale() = 0;
   virtual void set_scale(float factor) = 0;

   virtual void click(MouseButton button, MouseButtonState state, sf::Vector2f pos) = 0;
};

#endif
