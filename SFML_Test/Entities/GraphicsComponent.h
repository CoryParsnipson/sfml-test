#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include "Viewport.h"

// forward declarations
class Entity;

class GraphicsComponent {
public:
   virtual void update(Entity* entity, Viewport& viewport) = 0;
};

#endif
