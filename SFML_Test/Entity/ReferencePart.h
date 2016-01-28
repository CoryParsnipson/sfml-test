#ifndef REFERENCE_H
#define REFERENCE_H

#include "Part.h"

class ReferencePart : public Part {
public:
   typedef std::vector<Entity*> entities_t;

   ReferencePart(std::string id = "reference");
   virtual ~ReferencePart();

   virtual entities_t get();

   virtual void add(Entity* entity);
   virtual void remove(Entity* entity);

   virtual void update(Entity& entity, Scene& scene, Viewport& viewport);

protected:
   entities_t references_;
};

#endif
