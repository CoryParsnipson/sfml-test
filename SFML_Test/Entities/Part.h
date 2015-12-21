#ifndef PART_H
#define PART_H

#include "dependencies.h"
#include "Viewport.h"

// forward declarations
class Entity2;

class Part {
public:
   Part(std::string id) : id(id) {};
   virtual ~Part() {};

   std::string name() { return this->id; };

   // send message
   // receive message

   virtual void update(Entity2& entity, Viewport& viewport) = 0;

protected:
   std::string id;
};

#endif
