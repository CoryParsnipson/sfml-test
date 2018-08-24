#ifndef GRAVITY_H
#define GRAVITY_H

#include <SFML/Graphics.hpp>

#include "Component.h"

class Gravity : public Component {
public:
   explicit Gravity(const std::string& id = "Gravity Component", sf::Vector2f direction = sf::Vector2f(0, 1), float magnitude = 1.f);
   Gravity(const Gravity& other);
   virtual ~Gravity();

   Gravity& operator=(const Gravity& other);
   void swap(Gravity& other);

   void direction(sf::Vector2f direction);
   const sf::Vector2f& direction() const;

   void magnitude(float magnitude);
   float magnitude() const;

private:
   sf::Vector2f direction_; // this should be a unit vector
   float magnitude_;
};

#endif
