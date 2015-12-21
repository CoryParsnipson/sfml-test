#include "Game.h"
#include "Entities/Map.h"
#include "Entities/MouseGraphicsComponent.h"
#include "InputController.h"
#include "Manager/TextureManager.h"

#include "GameStartMenuState.h"

Game::Game()
{
   Service::init(); // initialize service locator

   // initialize logging and register service (this should be done asap)
   this->full_logger_.console_start();
   this->full_logger_.get_logger("console")->disable_all_tags();
   this->full_logger_.get_logger("console")->set_tag("Viewport", false);
   this->full_logger_.get_logger("console")->set_tag("InputController", false);
   this->full_logger_.get_logger("console")->set_tag("GraphicsPart", true);
   this->full_logger_.get_logger("console")->enable();

   //this->full_logger_.file_start("log.txt");
   //this->full_logger_.get_logger("file")->set_tag("Viewport", false);
   //this->full_logger_.get_logger("file")->set_tag("InputController", false);

   Service::provide_logger(&this->full_logger_);
   
   // set up input
   this->input_.registerInputListener(this);
   Service::provide_input(&this->input_);
   
   // load fonts
   this->graphics.load_font("retro", "retro.ttf");

   // initialize mouse interface wrapper
   //this->m = new Mouse(new MouseGraphicsComponent());
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
      Service::get_input().pollEvents(*this);

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
