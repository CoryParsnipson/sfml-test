#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include "Graphics.h"

// forward declarations
class Entity;

class GraphicsComponent {
public:
   virtual void update(Entity* entity, Graphics& graphics) = 0;
};

#endif
