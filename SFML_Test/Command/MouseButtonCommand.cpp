#include "MouseButtonCommand.h"

MouseButtonCommand::MouseButtonCommand(sf::Event::MouseButtonEvent& e, MouseButtonCommand::STATE state)
: state(state)
, x(e.x)
, y(e.y)
{
   // convert to button
   switch (e.button) {
   case sf::Mouse::Button::Left: this->button = MOUSE_BUTTON::LEFT; break;
   case sf::Mouse::Button::Right: this->button = MOUSE_BUTTON::RIGHT; break;
   case sf::Mouse::Button::Middle: this->button = MOUSE_BUTTON::MIDDLE; break;
   case sf::Mouse::Button::XButton1: this->button = MOUSE_BUTTON::XBUTTON1; break;
   case sf::Mouse::Button::XButton2: this->button = MOUSE_BUTTON::XBUTTON2; break;
   default: throw "MouseButtonCommand::MouseButtonCommand - Unrecognized mouse button id."; break;
   }
}
