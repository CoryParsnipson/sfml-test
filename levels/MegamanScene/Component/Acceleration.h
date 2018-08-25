#ifndef ACCELERATION_H
#define ACCELERATION_H

#include <SFML/Graphics.hpp>

#include "Component.h"

class Acceleration : public Component {
public:
   explicit Acceleration(const std::string& id = "Acceleration Component", float x = 0.f, float y = 0.f);
   Acceleration(const Acceleration& other);
   virtual ~Acceleration();

   Acceleration& operator=(const Acceleration& other);
   void swap(Acceleration& other);

   void x(float x);
   float x() const;

   void y(float y);
   float y() const;

   void value(sf::Vector2f value);
   const sf::Vector2f& value() const;

private:
   sf::Vector2f acceleration_;
};

#endif
