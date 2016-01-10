#ifndef GRAPHICS_PART_H
#define GRAPHICS_PART_H

#include "Part.h"
#include "Texture.h"
#include "Viewport.h"

class GraphicsPart : public Part {
public:
   typedef std::vector<sf::Sprite*> sprite_list_t;
   typedef std::vector<sf::Shape*> shape_list_t;

   GraphicsPart(std::string id = "graphics", Viewport* hud_viewport = nullptr);
   virtual ~GraphicsPart();

   virtual void add(sf::Sprite* sprite);
   virtual void add(sf::Shape* shape);
   // virtual remove();

   void set_show_outline(bool show);
   void set_show_debug_text(bool show);

   void set_viewport(Viewport* v);

   virtual void update(Entity& entity, Viewport& viewport); // TODO: figure out how to unify update method signatures

protected:
   bool show_outline_;
   bool show_debug_text_;
   FontConfig font_debug_;
   
   Viewport* hud_viewport;

   sprite_list_t sprites_;
   shape_list_t shapes_;
};

#endif
