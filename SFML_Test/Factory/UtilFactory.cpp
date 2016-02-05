#include "UtilFactory.h"

#include "GraphicsPart.h"
#include "PhysicsPart.h"
#include "MouseControlPart.h"

// initialize static members 
UtilFactory* UtilFactory::inst_ = new UtilFactory();

UtilFactory* UtilFactory::inst() {
   return UtilFactory::inst_;
}

UtilFactory::UtilFactory() {
}

UtilFactory::~UtilFactory() {
}

Entity* UtilFactory::create_mouse() {
   Entity* mouse = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart();
   MouseControlPart* control_part = new MouseControlPart("control");
   
   sf::RectangleShape* cursor = new sf::RectangleShape(sf::Vector2f(6, 6));
   cursor->setFillColor(sf::Color::Red);

   graphics_part->add(cursor);
   graphics_part->set_show_debug_text(true);

   mouse->add(graphics_part);
   mouse->add(physics_part);
   mouse->add(control_part);
   
   return mouse;
}

Entity* UtilFactory::create_graphic(sf::Drawable* d, sf::FloatRect bounding_box) {
   Entity* e = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart("physics", bounding_box);

   graphics_part->add(d);

   e->add(graphics_part);
   e->add(physics_part);

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
