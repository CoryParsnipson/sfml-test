#ifndef DRAW_H
#define DRAW_H

#include "dependencies.h"
#include "RenderTarget.h"
#include "Layer.h"

class Draw {
public:
   Draw() : layer_(nullptr) {}
   virtual ~Draw() {
      // remove from layer
      this->layer(nullptr);
   }

   virtual void draw(RenderTarget& surface) = 0;

   virtual void layer(Layer* layer) {
      if (this->layer_) {
         this->layer_->remove(this);
      }

      this->layer_ = layer;

      if (this->layer_) {
         this->layer_->add(this);
      }
   }
   virtual Layer* layer() { return this->layer_; }

protected:
   Layer* layer_;
};

#endif
