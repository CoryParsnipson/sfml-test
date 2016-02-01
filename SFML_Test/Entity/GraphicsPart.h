#ifndef GRAPHICS_PART_H
#define GRAPHICS_PART_H

#include "Part.h"
#include "Texture.h"
#include "Viewport.h"

class GraphicsPart
: public Part
{
public:
   typedef std::vector<sf::Sprite*> SpriteList;
   typedef std::vector<sf::Shape*> ShapeList;
   typedef std::vector<sf::Text*> TextList;

   GraphicsPart(std::string id = "graphics");
   virtual ~GraphicsPart();

   virtual void add(sf::Sprite* sprite);
   virtual void add(sf::Shape* shape);
   virtual void add(sf::Text* text);
   // virtual remove();

   void set_show_outline(bool show);
   void set_show_debug_text(bool show);
   bool get_show_debug_text();

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   bool show_outline_;
   bool show_debug_text_;

   SpriteList sprites_;
   ShapeList shapes_;
   TextList texts_;
};

#endif
