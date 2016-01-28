#ifndef PART_H
#define PART_H

#include "dependencies.h"

// forward declarations
class Entity;
class Scene;
class Viewport;

class Part {
public:
   Part(std::string id) : id(id) {};
   virtual ~Part() {};

   std::string name() { return this->id; };

   // send message
   // receive message

   virtual void update(Entity& entity, Scene& scene, Viewport& viewport) = 0;

protected:
   std::string id;
};

#endif
