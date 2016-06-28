#include "dependencies.h"

#include "Game.h"
#include "Input.h"
#include "InputEvent.h"
#include "CloseInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

Input::Input(std::string id /* = "Input" */)
: Subject<InputEvent>(id)
{
}


void Input::poll_event(Game& game) {
   sf::Event sfml_event;
   InputEvent* event = nullptr;

   while (game.poll_event(sfml_event)) {
      switch (sfml_event.type) {
      case sf::Event::Closed:
         Service::get_logger().msg("Input", Logger::INFO, "Received close event.");
         event = new CloseInputEvent();
         break;
      case sf::Event::Resized:
         Service::get_logger().msg("Input", Logger::INFO, "Received resize event.");
         event = new ResizeInputEvent(sfml_event.size.width, sfml_event.size.height);
         break;
      case sf::Event::KeyPressed:
         Service::get_logger().msg("Input", Logger::INFO, "Received key press event.");
         event = new KeyPressInputEvent(
            static_cast<Key>(sfml_event.key.code), // convert from enum sf::Keyboard::Key to our enum class Key
            sfml_event.key.alt, sfml_event.key.control,
            sfml_event.key.shift,
            sfml_event.key.system
         );
         break;
      case sf::Event::MouseMoved:
         Service::get_logger().msg("Input", Logger::INFO, "Received mouse move event.");
         event = new MouseMoveInputEvent(sfml_event.mouseMove.x, sfml_event.mouseMove.y);
         break;
      case sf::Event::MouseWheelMoved:
         Service::get_logger().msg("Input", Logger::INFO, "Received mouse wheel event.");
         event = new MouseWheelInputEvent(
            sfml_event.mouseWheel.x,
            sfml_event.mouseWheel.y,
            sfml_event.mouseWheel.delta
         );
         break;
      case sf::Event::MouseButtonPressed:
         Service::get_logger().msg("Input", Logger::INFO, "Received mouse button pressed event.");
         event = new MouseButtonInputEvent(
            static_cast<MouseButton>(sfml_event.mouseButton.button),
            ButtonState::Pressed,
            sfml_event.mouseButton.x,
            sfml_event.mouseButton.y
         );
         break;
      case sf::Event::MouseButtonReleased:
         Service::get_logger().msg("Input", Logger::INFO, "Received mouse button released event.");
         event = new MouseButtonInputEvent(
            static_cast<MouseButton>(sfml_event.mouseButton.button),
            ButtonState::Released,
            sfml_event.mouseButton.x,
            sfml_event.mouseButton.y
         );
         break;
      default:
         Service::get_logger().msg("Input", Logger::WARNING, "Input received unknown input event.");
         return;
      }

      this->notify(*event);
   }

}
