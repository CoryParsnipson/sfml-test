#ifndef DRAW_H
#define DRAW_H

#include "dependencies.h"

class RenderSurface;

class Draw {
public:
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) = 0;
};

#endif
