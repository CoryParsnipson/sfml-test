#ifndef ENTITY_H
#define ENTITY_H

#include "dependencies.h"
#include "Graphics.h"
#include "GraphicsComponent.h"

class Entity {
public:
   Entity(GraphicsComponent* graphics) : graphics_(graphics) {};
   virtual ~Entity() {};
  
   virtual void update(Graphics& graphics) = 0;

protected:
   GraphicsComponent* graphics_;
};

#endif
