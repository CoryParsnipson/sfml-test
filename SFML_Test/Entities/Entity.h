#ifndef ENTITY_H
#define ENTITY_H

#include "dependencies.h"
#include "Viewport.h"
#include "GraphicsComponent.h"

class Entity {
public:
   Entity(GraphicsComponent* graphics) : graphics_(graphics) {};
   virtual ~Entity() {};
  
   virtual void update(Viewport& viewport) = 0;

protected:
   GraphicsComponent* graphics_;
};

#endif
