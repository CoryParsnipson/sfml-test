#ifndef DEBUG_PART_H
#define DEBUG_PART_H

#include "Part.h"

class DebugPart
: public Part
{
public:
   typedef std::vector<Graphic*> SpriteList;

   DebugPart(std::string id = "debug");
   virtual ~DebugPart();

   void show_text(bool show_text);
   bool show_text();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   // serialize interface
   virtual Serialize::SerialObj serialize();
   virtual void deserialize(Serialize::SerialObj& obj);

protected:
   bool show_text_;

   Graphic* pos_text_;
   SpriteList sprites_;
};

#endif
