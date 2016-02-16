#ifndef GRAPHICS_PART_H
#define GRAPHICS_PART_H

#include "Part.h"

class Graphics;
class Layer;
class Graphic;

class GraphicsPart
: public Part
{
public:
   typedef std::vector<Graphic*> SpriteList;

   GraphicsPart(std::string id = "graphics");
   virtual ~GraphicsPart();

   virtual void add(Graphic* graphic);
   virtual Graphic* get(int idx);
   // virtual remove();

   virtual void set_position(float x, float y);
   virtual void set_position(const sf::Vector2f& pos);

   virtual void set_size(float width, float height);
   virtual void set_size(const sf::Vector2f& size);

   void set_show_outline(bool show);
   void set_show_debug_text(bool show);
   bool get_show_debug_text();
   bool get_show_outline();

   // draw interface
   virtual void draw(Graphics& graphics, Layer& layer);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   bool show_outline_;
   bool show_debug_text_;

   SpriteList sprites_;
};

#endif
