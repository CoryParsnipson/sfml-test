#ifndef GRAPHICS_PART_H
#define GRAPHICS_PART_H

#include "Part.h"

class GraphicsPart
: public Part
{
public:
   using SpriteList = std::vector<Graphic*>;

   GraphicsPart(std::string id = "graphics");
   virtual ~GraphicsPart();

   virtual void add(Graphic* graphic);
   virtual Graphic* get(int idx);
   // virtual remove();

   virtual void set_position(float x, float y);
   virtual void set_position(const sf::Vector2f& pos);

   virtual void set_size(float width, float height);
   virtual void set_size(const sf::Vector2f& size);

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Game& g, std::string& d);

protected:
   SpriteList sprites_;
};

#endif
