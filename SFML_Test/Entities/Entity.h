#ifndef ENTITY_H
#define ENTITY_H

#include "dependencies.h"
#include "Viewport.h"

// forward declarations
class Part;

class Entity {
public:
   typedef std::map<std::string, Part*> part_list_t;

   Entity();
   virtual ~Entity();
   
   virtual void add(Part* part);
   virtual Part* get(std::string part_name);
   // remove part
   // get part

   virtual void update(Viewport& viewport);

protected:
   part_list_t parts_;
};

#endif
