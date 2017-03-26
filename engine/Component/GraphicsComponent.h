#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <iostream>
#include "Component.h"

class GraphicsComponent : public Component {
public:
   GraphicsComponent(std::string id = "GraphicsComponent", Graphic* graphic = nullptr);
   virtual ~GraphicsComponent();

   virtual void init();
   virtual void reset();

   void graphic(Graphic* graphic);
   const Graphic* graphic();

private:
   Graphic* graphic_;
};

#endif
