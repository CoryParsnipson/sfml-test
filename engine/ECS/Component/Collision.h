#ifndef COLLISION_H
#define COLLISION_H

#include "sfml.h"

#include "Component.h"

// ----------------------------------------------------------------------------
// Collision
//
// This component specifies a collision volume to use when determining things
// like mouse events in the Callback system or physics, etc.
// ----------------------------------------------------------------------------
class Collision : public Component {
public:
   explicit Collision(const std::string& id = "Collision Component", sf::FloatRect volume = sf::FloatRect(0, 0, 0, 0));
   Collision(const Collision& other);
   virtual ~Collision();

   Collision& operator=(const Collision& other);
   void swap(Collision& other);

   bool contains(sf::Vector2f point) const;
   bool intersects(sf::FloatRect other) const;

   void volume(sf::FloatRect volume);
   void volume(sf::Vector2f pos, sf::Vector2f size);

   const sf::FloatRect& volume() const;

private:
   sf::FloatRect volume_;
};

#endif

