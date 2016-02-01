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
   
   virtual void add(Part* part);
   // virtual void remove()
   virtual Part* get(std::string part_name);

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   std::string name_;
   PartList parts_;
};

#endif
