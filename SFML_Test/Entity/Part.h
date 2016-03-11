#ifndef PART_H
#define PART_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"
#include "Serialize.h"

class Viewport;

class Part
: public Draw
, public Update
, public Serialize
{
public:
   Part(std::string id) : id(id) {};
   virtual ~Part() {};

   std::string name() { return this->id; };

   virtual void set_position(float x, float y) {}
   virtual void set_position(const sf::Vector2f& pos) {}

   virtual void set_size(float width, float height) {}
   virtual void set_size(const sf::Vector2f& size) {}

   virtual bool intersects(sf::Vector2i& other) { return false; }
   virtual bool intersects(sf::Vector2f& other) { return false; }
   virtual bool intersects(sf::FloatRect& other) { return false; }

   // send message ???
   // receive message ???

   // draw interface
   virtual void draw(Graphics& graphics) {}

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

   // serialize interface
   virtual Serialize::SerialObj serialize() = 0;
   virtual void deserialize(Serialize::SerialObj& obj) = 0;

protected:
   std::string id;
};

#endif
