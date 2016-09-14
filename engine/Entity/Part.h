#ifndef PART_H
#define PART_H

#include <string>

#include "sfml.h"

#include "Draw.h"
#include "Update.h"
#include "Serialize.h"

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

   virtual bool intersects(const sf::Vector2i& other) { return false; }
   virtual bool intersects(const sf::Vector2f& other) { return false; }
   virtual bool intersects(const sf::FloatRect& other) { return false; }

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {}

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr) = 0;

   // serialize interface
   virtual Serialize::SerialObj serialize() = 0;
   virtual void deserialize(Serialize::SerialObj& obj, const TextureAtlas* textures = nullptr) = 0;

protected:
   std::string id;
};

#endif
