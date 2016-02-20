#ifndef DEBUG_PART_H
#define DEBUG_PART_H

#include "Part.h"

class Graphics;
class Graphic;

class DebugPart
: public Part
{
public:
   typedef std::vector<Graphic*> SpriteList;
   
   DebugPart(std::string id = "debug");
   virtual ~DebugPart();

   // draw interface
   virtual void draw(Graphics& graphics);
   virtual void layer(Layer* layer);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   Graphic* pos_text_;
   SpriteList sprites_;
};

#endif
