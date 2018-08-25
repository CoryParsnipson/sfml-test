#ifndef GRAVITY_H
#define GRAVITY_H

#include <SFML/Graphics.hpp>

#include "Component.h"

class Gravity : public Component {
public:
   explicit Gravity(const std::string& id = "Gravity Component", sf::Vector2f gravity = sf::Vector2f(0, 1));
   Gravity(const Gravity& other);
   virtual ~Gravity();

   Gravity& operator=(const Gravity& other);
   void swap(Gravity& other);

   void value(sf::Vector2f gravity);
   const sf::Vector2f& value() const;

private:
   sf::Vector2f direction_; // this should be a unit vector
};

#endif
