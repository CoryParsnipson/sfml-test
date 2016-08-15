#ifndef PHYSICS_H
#define PHYSICS_H

#include "Part.h"

class PhysicsPart : public Part {
public:
   PhysicsPart(std::string id = "physics", sf::FloatRect bounding_box = sf::FloatRect(0, 0, 0, 0));
   virtual ~PhysicsPart();

   virtual void set_position(float x, float y);
   virtual void set_position(const sf::Vector2f& pos);

   sf::Vector2f get_position();

   virtual void set_size(float width, float height);
   virtual void set_size(const sf::Vector2f& size);

   sf::Vector2f get_size();

   sf::FloatRect get_bounding_box();

   void set_solid(bool solid);
   bool is_solid();

   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

   // serialize interface
   virtual Serialize::SerialObj serialize();
   virtual void deserialize(Serialize::SerialObj& obj, const TextureDictionary* textures = nullptr);

protected:
   bool solid_; // let's try this...
   sf::FloatRect bounding_box_; // position of this box is world coordinates
};

#endif
