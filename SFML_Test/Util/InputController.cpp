#include "InputController.h"
#include "../Command/CloseCommand.h"
#include "../Command/KeyPressCommand.h"
#include "../Command/WindowResizeCommand.h"
#include "../Command/MouseMoveCommand.h"
#include "../Command/MouseButtonCommand.h"
#include "../Command/MouseWheelCommand.h"

#include "Game.h"

InputController& InputController::instance() {
   static InputController _inst;
   return _inst;
}

InputController::InputController() {
}

void InputController::registerInputListener(InputListener* listener) {
   this->listeners.push_back(listener);
}

void InputController::pollEvents(Game& game) {
   sf::Event event;
   Command* c = 0;

   while (game.graphics.get_window().pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
         Service::get_logger().msg(Logger::HIGH, Logger::INFO, "InputController::pollEvents: close event received");
         c = new CloseCommand();
         break;
      case sf::Event::Resized:
         Service::get_logger().msg(Logger::HIGH, Logger::INFO, "InputController::pollEvents: resize event received");
         c = new WindowResizeCommand(event.size);
         break;
      case sf::Event::KeyPressed:
         Service::get_logger().msg(Logger::HIGH, Logger::INFO, "InputController::pollEvents: keypress event received");
         c = new KeyPressCommand(event.key);
         break;
      case sf::Event::MouseMoved:
         Service::get_logger().msg(Logger::HIGH, Logger::INFO, "InputController::pollEvents: mouse move event received");
         c = new MouseMoveCommand(event.mouseMove);
         break;
      case sf::Event::MouseButtonPressed:
         Service::get_logger().msg(Logger::HIGH, Logger::INFO, "InputController::pollEvents: mouse button (pressed) event received");
         c = new MouseButtonCommand(event.mouseButton, MouseButtonCommand::PRESSED);
         break;
      case sf::Event::MouseButtonReleased:
         Service::get_logger().msg(Logger::HIGH, Logger::INFO, "InputController::pollEvents: mouse button (released) event received");
         c = new MouseButtonCommand(event.mouseButton, MouseButtonCommand::RELEASED);
         break;
      case sf::Event::MouseWheelMoved:
         Service::get_logger().msg(Logger::HIGH, Logger::INFO, "InputController::pollEvents: mouse wheel event received");
         c = new MouseWheelCommand(event.mouseWheel);
         break;
      default:
         // just drop event
         return;
      }
   
      if (!c) {
         Service::get_logger().msg(Logger::MEDIUM, Logger::WARNING, "InputController::pollEvents: command object not instantiated.");
         return;
      }
      
      // dispatch event to game entities
      std::vector<InputListener*>::iterator it = this->listeners.begin();
      for (it = this->listeners.begin(); it != this->listeners.end(); it++) {
         c->execute(*(*it));
      }
   }
}
