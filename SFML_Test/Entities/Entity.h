#ifndef ENTITY_H
#define ENTITY_H

#include "dependencies.h"

// forward declarations
class Part;
class Scene;
class Viewport;

class Entity {
public:
   typedef std::map<std::string, Part*> part_list_t;

   Entity(std::string name = "entity");
   virtual ~Entity();
   
   virtual void add(Part* part);
   virtual Part* get(std::string part_name);
   // remove part
   // get part

   virtual void update(Scene& scene, Viewport& viewport);

   std::string to_string();

protected:
   std::string name_;
   part_list_t parts_;
};

#endif
