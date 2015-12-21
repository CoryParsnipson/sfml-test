#include "UtilFactory.h"

#include "Entity2.h"
#include "GraphicsPart.h"
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

Entity2* UtilFactory::create_mouse() {
   Entity2* mouse = new Entity2();
   GraphicsPart* graphics_part = new GraphicsPart();
   MouseControlPart* control_part = new MouseControlPart();
   
   sf::RectangleShape* cursor = new sf::RectangleShape(sf::Vector2f(6, 6));
   cursor->setFillColor(sf::Color::Red);

   graphics_part->add(cursor);

   mouse->add(graphics_part);
   mouse->add(control_part);
   
   return mouse;
}
