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

std::string MouseButtonCommand::to_string() {
   std::string button_str;

   switch (this->button) {
   case LEFT: button_str = "LEFT"; break;
   case RIGHT: button_str = "RIGHT"; break;
   case MIDDLE: button_str = "MIDDLE"; break;
   case XBUTTON1: button_str = "XBUTTON1"; break;
   case XBUTTON2: button_str = "XBUTTON2"; break;
   }

   return "[MouseButtonCommand: " +
          button_str + ", " +
          (state == PRESSED ? "PRESSED " : "RELEASED ") + 
          "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")]";
}
