#include "GraphicsPart.h"
#include "FontConfig.h"

GraphicsPart::GraphicsPart()
: font_debug_("retro", 10, FontConfig::ALIGN::LEFT)
{
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart");
}

GraphicsPart::~GraphicsPart() {
   this->sprites_.clear();
   this->shapes_.clear();
}

void GraphicsPart::add(sf::Sprite* sprite) {
   this->sprites_.push_back(sprite);
}

void GraphicsPart::add(sf::Shape* shape) {
   this->shapes_.push_back(shape);
}

void GraphicsPart::update(Entity2& entity, Viewport& viewport) {
   sf::Vector2f pos(100, 100);

   sprite_list_t::iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); sprite_it++) {
      // draw outline for this sprite
      sf::RectangleShape s(sf::Vector2f((*sprite_it)->getGlobalBounds().width, (*sprite_it)->getGlobalBounds().height));      

      s.setPosition(pos);
      s.setOutlineThickness(1);
      s.setOutlineColor(sf::Color::Red);
      s.setFillColor(sf::Color::Transparent);
      
      (*sprite_it)->setPosition(pos);
      
      viewport.draw(*(*sprite_it));
      viewport.draw(s);
   }

   shape_list_t::iterator shape_it;
   for (shape_it = this->shapes_.begin(); shape_it != this->shapes_.end(); shape_it++) {
      (*shape_it)->setPosition(pos);
      viewport.draw(*(*shape_it));
   }

   // draw diagnostic info
   viewport.write("100, 100", pos - sf::Vector2f(0, 11), &this->font_debug_);
}
