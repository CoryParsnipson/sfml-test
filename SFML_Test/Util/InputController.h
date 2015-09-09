#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "../dependencies.h"
#include "Entities/GameEntity.h"

// this class is an interface between SFML and the proprietary
// input handler class and command hierarchy
class InputController {
public:
   static InputController& instance(); // object access method

   void registerInputListener(GameEntity* entity);
   void pollEvents(sf::RenderWindow& window);

private:
   // seal off constructors
   InputController();
   InputController(InputController const &) = delete;
   void operator=(InputController const &) = delete;

   std::vector<GameEntity*> listeners;
};

#endif
