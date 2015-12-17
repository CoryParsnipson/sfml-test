#ifndef GRAPHICS_PART_H
#define GRAHPICS_PART_H

#include "Part.h"
#include "Texture.h"
#include "Viewport.h"

class GraphicsPart {
public:
   //GraphicsPart(Texture& t, FontConfig* fc = nullptr);
   GraphicsPart(Texture* t);

   // this calls parent update method?
   virtual void update(Entity2& entity, Viewport& viewport);

protected:
   FontConfig font_debug_;

   Texture* texture_;
   sf::Sprite sprite_;
};

#endif
