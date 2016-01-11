#ifndef MOUSE_UTIL_H
#define MOUSE_UTIL_H

// abstract interface to define actions that mouse control options
class MouseControllable {
public:
   virtual void drag(sf::Vector2f delta) = 0;

   virtual float get_scale() = 0;
   virtual void set_scale(float factor) = 0;
};

#endif
