#ifndef ENTITY_H
#define ENTITY_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"

// forward declarations
class Part;

class Entity
: public Draw
, public Update
{
public:
   typedef std::map<std::string, Part*> PartList;

   Entity(std::string name = "entity");
   virtual ~Entity();

   std::string to_string();

   void set_position(float x, float y);
   void set_position(const sf::Vector2f& pos);

   void set_size(float width, float height);
   void set_size(const sf::Vector2f& size);
   
   bool intersects(sf::Vector2i& other);
   bool intersects(sf::Vector2f& other);
   bool intersects(sf::FloatRect& other);
   //bool intersects(Entity& other); // TODO

   // part management interface
   void add(Part* part);
   void remove(const std::string& part_id);
   Part* get(const std::string& part_name);

   // draw interface
   virtual void draw(Graphics& graphics);
   virtual void layer(Layer* layer);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   std::string name_;
   PartList parts_;
};

#endif
