#ifndef PART_H
#define PART_H

#include "dependencies.h"

#include "Draw.h"
#include "Update.h"

class Viewport;

class Part
: public Draw
, public Update
{
public:
   Part(std::string id) : id(id) {};
   virtual ~Part() {};

   std::string name() { return this->id; };

   // send message
   // receive message

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer) {}

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

protected:
   std::string id;
};

#endif
