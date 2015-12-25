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

Entity* UtilFactory::create_mouse(Viewport* graphics_viewport) {
   Entity* mouse = new Entity();
   GraphicsPart* graphics_part = new GraphicsPart();
   PhysicsPart* physics_part = new PhysicsPart();
   MouseControlPart* control_part = new MouseControlPart();
   
   sf::RectangleShape* cursor = new sf::RectangleShape(sf::Vector2f(6, 6));
   cursor->setFillColor(sf::Color::Red);

   graphics_part->add(cursor);
   graphics_part->set_viewport(graphics_viewport);

   mouse->add(graphics_part);
   mouse->add(physics_part);
   mouse->add(control_part);
   
   return mouse;
}
