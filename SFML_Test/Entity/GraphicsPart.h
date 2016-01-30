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

   void set_show_outline(bool show);
   void set_show_debug_text(bool show);
   bool get_show_debug_text();

   // drawable interface...
   virtual void draw(Viewport& viewport);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   bool show_outline_;
   bool show_debug_text_;
   FontConfig font_debug_;
   
   sprite_list_t sprites_;
   shape_list_t shapes_;
};

#endif
