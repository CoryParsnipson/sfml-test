#ifndef DRAW_H
#define DRAW_H

#include "dependencies.h"

class Graphics;

class Draw {
public:
   virtual void draw(Graphics& graphics) = 0;
};

#endif
