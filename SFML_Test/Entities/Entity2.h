#ifndef ENTITY2_H
#define ENTITY2_H

#include "dependencies.h"
#include "Viewport.h"

// forward declarations
class Part;

class Entity2 {
public:
   typedef std::map<std::string, Part*> part_list_t;

   Entity2();
   virtual ~Entity2();
   
   virtual void add(Part* part);
   virtual Part* get(std::string part_name);
   // remove part
   // get part

   virtual void update(Viewport& viewport);

protected:
   sf::Vector2f pos; // world location // move to physics component?

   part_list_t parts_;
};

#endif
