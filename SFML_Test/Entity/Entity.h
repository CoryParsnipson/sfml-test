#ifndef ENTITY_H
#define ENTITY_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"
#include "Serialize.h"

// forward declarations
class Part;

class Entity
: public Draw
, public Update
, public Serialize
{
public:
   typedef std::map<std::string, Part*> PartList;

   Entity(std::string name = "entity");
   virtual ~Entity();

   std::string to_string();

   void enable_debug_wireframe(bool debug_wireframe);
   bool enable_debug_wireframe();

   void enable_debug_text(bool debug_text);
   bool enable_debug_text();

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
   virtual void draw(RenderTarget& surface);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   // serialize interface
   virtual Serialize::SerialObj serialize();
   virtual void deserialize(Serialize::SerialObj& obj);

protected:
   bool enable_debug_wireframe_;
   bool enable_debug_text_;

   std::string name_;
   PartList parts_;
};

#endif
