#include "InputController.h"
#include "../Command/CloseCommand.h"
#include "../Command/KeyPressCommand.h"
#include "../Command/WindowResizeCommand.h"
#include "../Command/MouseMoveCommand.h"
#include "../Command/MouseButtonCommand.h"
#include "../Command/MouseWheelCommand.h"

InputController& InputController::instance() {
   static InputController _inst;
   return _inst;
}

InputController::InputController() {
}

void InputController::registerInputListener(InputListener* listener) {
   this->listeners.push_back(listener);
}

void InputController::pollEvents(sf::RenderWindow& window) {
   sf::Event event;
   Command* c = 0;

   while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
         std::cout << "InputController::pollEvents -> close event received" << std::endl;
         c = new CloseCommand();
         break;
      case sf::Event::Resized:
         std::cout << "InputController::pollEvents -> size event received" << std::endl;
         c = new WindowResizeCommand(event.size);
         break;
      case sf::Event::KeyPressed:
         std::cout << "InputController::pollEvents -> keypress event received" << std::endl;
         c = new KeyPressCommand(event.key);
         break;
      case sf::Event::MouseMoved:
         std::cout << "InputController::pollEvents -> mouse move event received" << std::endl;
         c = new MouseMoveCommand(event.mouseMove);
         break;
      case sf::Event::MouseButtonPressed:
         std::cout << "InputController::pollEvents -> mouse button event received (pressed)" << std::endl;
         c = new MouseButtonCommand(event.mouseButton, MouseButtonCommand::PRESSED);
         break;
      case sf::Event::MouseButtonReleased:
         std::cout << "InputController::pollEvents -> mouse button event received (released)" << std::endl;
         c = new MouseButtonCommand(event.mouseButton, MouseButtonCommand::RELEASED);
         break;
      case sf::Event::MouseWheelMoved:
         std::cout << "InputController::pollEvents -> mouse wheel moved event received" << std::endl;
         c = new MouseWheelCommand(event.mouseWheel);
         break;
      default:
         // just drop event
         return;
      }
   
      if (!c) {
         std::cout << "InputController::pollEvents -> command object not instantiated" << std::endl;
         return;
      }
      
      // dispatch event to game entities
      std::vector<InputListener*>::iterator it = this->listeners.begin();
      for (it = this->listeners.begin(); it != this->listeners.end(); it++) {
         c->execute(*(*it));
      }
   }
}
