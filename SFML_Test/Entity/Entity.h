#ifndef ENTITY_H
#define ENTITY_H

#include "dependencies.h"

#include "Update.h"

// forward declarations
class Part;
class Viewport;

class Entity
: public Update
{
public:
   typedef std::map<std::string, Part*> part_list_t;

   Entity(std::string name = "entity");
   virtual ~Entity();
   
   virtual void add(Part* part);
   // virtual void remove()
   virtual Part* get(std::string part_name);

   // drawable interface
   virtual void draw(Viewport& viewport);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   std::string to_string();

protected:
   std::string name_;
   part_list_t parts_;
};

#endif
