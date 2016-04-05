#ifndef DRAW_H
#define DRAW_H

#include "dependencies.h"
#include "RenderTarget.h"

class Draw {
public:
   virtual void draw(RenderTarget& surface) = 0;
};

#endif
