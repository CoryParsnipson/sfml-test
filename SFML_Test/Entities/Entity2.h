#ifndef ENTITY2_H
#define ENTITY2_H

#include "dependencies.h"
#include "Viewport.h"

// forward declarations
class Part;

class Entity2 {
public:
   typedef std::vector<Part*> part_list_t;

   Entity2();
   virtual ~Entity2();
   
   // add part
   virtual void add(Part* part);

   // remove part
   // get part
  
   // iterator
   //Iterator* get_iterator();

   virtual void update(Viewport& viewport);

protected:
   sf::Vector2f pos; // world location // move to physics component?

   part_list_t parts_;
};

#endif
