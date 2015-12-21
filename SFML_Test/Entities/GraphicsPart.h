#ifndef GRAPHICS_PART_H
#define GRAPHICS_PART_H

#include "Part.h"
#include "Texture.h"
#include "Viewport.h"

class GraphicsPart : public Part {
public:
   typedef std::vector<sf::Sprite*> sprite_list_t;
   typedef std::vector<sf::Shape*> shape_list_t;

   GraphicsPart(std::string id = "graphics");
   virtual ~GraphicsPart();

   virtual void add(sf::Sprite* sprite);
   virtual void add(sf::Shape* shape);
   // virtual remove();

   virtual void update(Entity2& entity, Viewport& viewport); // TODO: figure out how to unify update method signatures

protected:
   FontConfig font_debug_;

   sprite_list_t sprites_;
   shape_list_t shapes_;
};

#endif
