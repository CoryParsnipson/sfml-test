#ifndef DRAW_H
#define DRAW_H

#include "dependencies.h"

class Graphics;
class Layer;

class Draw {
public:
   virtual void draw(Graphics& graphics, Layer& layer) = 0;
};

#endif
