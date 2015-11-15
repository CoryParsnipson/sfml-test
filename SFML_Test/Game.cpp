#include "Game.h"
#include "Entities/Map.h"
#include "Util/InputController.h"

#include "GameStartMenuState.h"

Game::Game()
: InputListener()
, window(sf::VideoMode(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT), "SFML Test")
, sw(this->window)
{
	this->window.setFramerateLimit(Settings::Instance()->FRAMERATE_LIMIT);
	this->window.setMouseCursorVisible(false);

	this->sw.load_font("retro", "retro.ttf");

   // initialize mouse interface wrapper
   this->m = new Mouse(window);

   // set up input controller
   InputController& ic = InputController::instance();
   ic.registerInputListener(this);
   ic.registerInputListener(this->m);
}

Game::~Game() {
}

void Game::reset() {
   // specify explicit start state
   this->state_ = &GameState::start_menu_state;
   this->state_->enter(*this);

   // start game loop
   this->main_loop();
}

void Game::main_loop() {
   while (this->window.isOpen()) {
      // poll input
      InputController& ic = InputController::instance();
      ic.pollEvents(this->window);

      GameState* state = this->state_->update(*this);
      if (state != NULL) {
         this->state_->exit(*this);
         this->state_ = state;
         this->state_->enter(*this);
      }
   }
}

void Game::exit() {
   this->window.close();
}

void Game::process(CloseCommand& c) {
   this->state_->process(*this, c);
}

void Game::process(KeyPressCommand& c) {
   this->state_->process(*this, c);
}

void Game::process(WindowResizeCommand& c) {
   this->state_->process(*this, c);
}

void Game::process(MouseMoveCommand& c) {
   this->state_->process(*this, c);
}

void Game::process(MouseButtonCommand& c) {
   this->state_->process(*this, c);
}

void Game::process(MouseWheelCommand& c) {
   this->state_->process(*this, c);
}
