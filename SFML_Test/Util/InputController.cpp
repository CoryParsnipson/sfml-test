#include "InputController.h"
#include "../Command/CloseCommand.h"
#include "../Command/KeyPressCommand.h"
#include "../Command/WindowResizeCommand.h"

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
