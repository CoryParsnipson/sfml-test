#ifndef DRAW_H
#define DRAW_H

#include "dependencies.h"

class RenderSurface;

class Draw {
public:
   Draw() : layer_(0) {}
   virtual ~Draw() {}

   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) = 0;

   virtual int layer() const { return this->layer_; }
   virtual void layer(int layer) { this->layer_ = layer; }

protected:
   int layer_;
};

#endif
