#include "UtilFactory.h"

#include "Draw.h"
#include "GraphicsPart.h"
#include "PhysicsPart.h"
#include "DebugPart.h"

// initialize static members
UtilFactory* UtilFactory::inst_ = new UtilFactory();

UtilFactory* UtilFactory::inst() {
   return UtilFactory::inst_;
}

UtilFactory::UtilFactory() {}
UtilFactory::~UtilFactory() {}

Entity* UtilFactory::create_graphic(Graphic* g, sf::FloatRect bounding_box, bool debug) {
   Entity* e = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart("physics", bounding_box);

   graphics_part->add(g);

   e->add(graphics_part);
   e->add(physics_part);

   if (debug) {
      e->add(new DebugPart());
   }

   e->set_position(bounding_box.left, bounding_box.top);

   return e;
}

sf::FloatRect* UtilFactory::create_float_rect(sf::Vector2f& one, sf::Vector2f& two) {
   sf::Vector2f pos;
   sf::Vector2f end;

   pos.x = std::min(one.x, two.x);
   pos.y = std::min(one.y, two.y);

   end.x = std::max(one.x, two.x);
   end.y = std::max(one.y, two.y);

   return new sf::FloatRect(pos, end - pos);
}
