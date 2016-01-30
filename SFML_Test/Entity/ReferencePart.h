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

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   entities_t references_;
};

#endif
