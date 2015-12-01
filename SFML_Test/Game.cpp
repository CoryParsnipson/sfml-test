#include "Game.h"
#include "Entities/Map.h"
#include "Entities/MouseGraphicsComponent.h"
#include "Util/InputController.h"
#include "Manager/TextureManager.h"

#include "GameStartMenuState.h"

Game::Game()
{
   Service::init(); // initialize service locator

   // initialize logging and register service (this should be done first)
   this->full_logger_.console_start();
   this->full_logger_.file_start("log.txt");

   Service::provide_logger(&this->full_logger_);
   
   // load fonts
   this->graphics.load_font("retro", "retro.ttf");

   // initialize mouse interface wrapper
   this->m = new Mouse(new MouseGraphicsComponent());

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
   while (this->graphics.is_open()) {
      // poll input
      InputController& ic = InputController::instance();
      ic.pollEvents(*this);

      // clear graphics
      this->graphics.clear();

      GameState* state = this->state_->update(*this);
      if (state != NULL) {
         this->state_->exit(*this);
         this->state_ = state;
         this->state_->enter(*this);
      }

      // update graphics
      this->graphics.update();
   }
}

void Game::exit() {
   this->graphics.close();
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
