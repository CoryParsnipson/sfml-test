#include "InputController.h"
#include "../Command/CloseCommand.h"

InputController& InputController::instance() {
   static InputController _inst;
   return _inst;
}

InputController::InputController() {
}

void InputController::registerInputListener(GameEntity* entity) {
   this->listeners.push_back(entity);
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
      std::vector<GameEntity*>::iterator it = this->listeners.begin();
      for (it = this->listeners.begin(); it != this->listeners.end(); it++) {
         c->execute(*(*it));
      }
   }
}
