#ifndef PHYSICS_H
#define PHYSICS_H

#include "Part.h"

class PhysicsPart : public Part {
public:
   PhysicsPart(std::string id = "physics", sf::FloatRect bounding_box = sf::FloatRect(0, 0, 0, 0));
   virtual ~PhysicsPart();

   void set_position(sf::Vector2f pos);
   void set_position(float x, float y);

   sf::Vector2f get_position();

   void set_size(sf::Vector2f size);
   void set_size(float width, float height);
   
   sf::Vector2f get_size();

   sf::FloatRect get_bounding_box();
   
   void set_solid(bool solid);
   bool is_solid();

   bool intersects(sf::Vector2i other_point);
   bool intersects(sf::Vector2f other_point);
   bool intersects(sf::FloatRect other_bounding_box);

   virtual void update(Entity& entity, Scene& scene, Viewport& viewport);

protected:
   bool solid_; // let's try this...
   sf::FloatRect bounding_box_; // position of this box is world coordinates
};

#endif
