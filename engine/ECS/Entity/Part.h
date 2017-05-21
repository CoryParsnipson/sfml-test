#ifndef PART_H
#define PART_H

#include <string>

#include "sfml.h"

#include "Draw.h"
#include "Update.h"
#include "Serializable.h"

class Part
: public Draw
, public Update
, public Serializable
{
public:
   Part(std::string id) : id_(id) {};
   virtual ~Part() {};

   std::string id() { return this->id_; };

   virtual void set_position(float x, float y) {}
   virtual void set_position(const sf::Vector2f& pos) {}

   virtual void set_size(float width, float height) {}
   virtual void set_size(const sf::Vector2f& size) {}

   virtual bool intersects(const sf::Vector2i& other) { return false; }
   virtual bool intersects(const sf::Vector2f& other) { return false; }
   virtual bool intersects(const sf::FloatRect& other) { return false; }

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}

   // update interface
   virtual void update(Game& game) = 0;

   // serialize interface
   virtual std::string serialize(Serializer& s) = 0;
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d) = 0;

protected:
   std::string id_;
};

#endif
