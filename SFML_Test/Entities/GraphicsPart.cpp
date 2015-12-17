#include "GraphicsPart.h"
#include "FontConfig.h"

GraphicsPart::GraphicsPart(Texture* t)
: font_debug_("retro", 12, FontConfig::ALIGN::LEFT)
, texture_(t)
{
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart.");
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Setting texture to " + this->texture_->to_string());

   this->sprite_.setTexture(this->texture_->get_texture());
}

void GraphicsPart::update(Entity2& entity, Viewport& viewport) {
   sf::Vector2f pos(100, 100);
   sf::Vector2f bounds(this->sprite_.getGlobalBounds().width, this->sprite_.getGlobalBounds().height);
   sf::RectangleShape s(bounds);

   this->sprite_.setPosition(pos);
   s.setPosition(pos);
   s.setFillColor(sf::Color::Transparent);

   s.setOutlineThickness(1);
   s.setOutlineColor(sf::Color::Red);

   viewport.draw(this->sprite_);
   viewport.draw(s);

   viewport.write("100, 100", pos, &this->font_debug_);
}
