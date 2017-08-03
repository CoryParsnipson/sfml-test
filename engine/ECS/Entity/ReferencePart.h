#ifndef REFERENCE_H
#define REFERENCE_H

#include "Part.h"

class Entity;

class ReferencePart : public Part {
public:
   typedef std::vector<Entity*> entities_t;

   ReferencePart(std::string id = "reference");
   virtual ~ReferencePart();

   virtual entities_t get();

   virtual void add(Entity* entity);
   virtual void remove(Entity* entity);

   // update interface
   virtual void update(Game& game);

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

protected:
   entities_t references_;
};

#endif