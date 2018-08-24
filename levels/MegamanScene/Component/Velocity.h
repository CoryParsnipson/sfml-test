#ifndef VELOCITY_H
#define VELOCITY_H

#include <SFML/Graphics.hpp>

#include "Component.h"

class Velocity : public Component {
public:
   explicit Velocity(const std::string& id = "Velocity Component", float x = 0.f, float y = 0.f);
   Velocity(const Velocity& other);
   virtual ~Velocity();

   Velocity& operator=(const Velocity& other);
   void swap(Velocity& other);

   void x(float x);
   float x() const;

   void y(float y);
   float y() const;

   void value(sf::Vector2f value);
   const sf::Vector2f& value() const;

private:
   sf::Vector2f velocity_;
};

#endif
