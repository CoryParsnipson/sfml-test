#ifndef PHYSICS_H
#define PHYSICS_H

#include "Part.h"

class PhysicsPart : public Part {
public:
   PhysicsPart(std::string id = "physics");
   virtual ~PhysicsPart();

   void set_position(sf::Vector2f pos);
   void set_position(float x, float y);
   sf::Vector2f get_position();

   virtual void update(Entity& entity, Viewport& viewport);

protected:
   sf::Vector2f pos; // world coordinates
};

#endif
