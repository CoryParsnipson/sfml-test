#include "Game.h"
#include "Util/InputController.h"

#include "GameStartMenuState.h"

// initialize static instance
GameStartMenuState GameState::start_menu_state;

GameStartMenuState::GameStartMenuState()
: is_running(true)
{
}

GameStartMenuState::~GameStartMenuState() {
}

void GameStartMenuState::enter(Game& game) {
   std::cout << "Entering game start menu state." << std::endl;
}

void GameStartMenuState::exit(Game& game) {
}

GameState* GameStartMenuState::update(Game& game) {
   std::cout << "Updating GameStartMenuState state." << std::endl;

   while (game.window.isOpen()) {
      InputController& ic = InputController::instance();
      ic.pollEvents(game.window);

      if (!this->is_running) {
         game.window.close();
      }
   }

   return NULL;
}

void GameStartMenuState::process(CloseCommand& c) {
   std::cout << "[GameStartMenuState] Close Command" << std::endl;
   
   this->is_running = false;
}

void GameStartMenuState::process(KeyPressCommand& c) {
   std::cout << "[GameStartMenuState] KeyPress Command" << std::endl;
}

void GameStartMenuState::process(WindowResizeCommand& c) {
   std::cout << "[GameStartMenuState] WindowResize Command" << std::endl;
}

void GameStartMenuState::process(MouseMoveCommand& c) {
   std::cout << "[GameStartMenuState] MouseMove Command" << std::endl;
}

void GameStartMenuState::process(MouseButtonCommand& c) {
   std::cout << "[GameStartMenuState] MouseButton Command" << std::endl;
}

void GameStartMenuState::process(MouseWheelCommand& c) {
   std::cout << "[GameStartMenuState] MouseWheel Command" << std::endl;
}
