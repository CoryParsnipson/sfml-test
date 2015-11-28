#include "MouseGraphicsComponent.h"
#include "Mouse.h"

MouseGraphicsComponent::MouseGraphicsComponent()
: font_cursor("retro", 12)
, cursor(sf::Vector2f(6, 6))
{
	this->cursor.setFillColor(sf::Color::Red);
   this->set_position(sf::Mouse::getPosition());
}

void MouseGraphicsComponent::update(Entity* entity, Viewport& viewport) {
   sf::Vector2i mouse_pos = dynamic_cast<Mouse*>(entity)->get_mouse_position();
   this->set_position(mouse_pos);

   // draw cursor
   viewport.draw(this->cursor);

   // draw position information
   std::stringstream pos_info;

   pos_info << mouse_pos.x << ", " << mouse_pos.y;

   viewport.write(pos_info.str(), static_cast<sf::Vector2f>(mouse_pos) + sf::Vector2f(0, 8), &this->font_cursor);
}

void MouseGraphicsComponent::set_position(sf::Vector2f pos) {
   this->cursor.setPosition(pos);
}

void MouseGraphicsComponent::set_position(sf::Vector2i pos) {
   this->set_position(static_cast<sf::Vector2f>(pos));
}
