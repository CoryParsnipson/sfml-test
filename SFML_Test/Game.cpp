#include "Game.h"

#include "InputController.h"
#include "TextureManager.h"

Game::Game()
: is_running_(false)
, next_scene_(nullptr)
, prev_scene_(nullptr)
{
   Service::init(); // initialize service locator

   // initialize logging and register service (this should be done asap)
   this->full_logger_.console_start();
   this->full_logger_.get_logger("console")->set_tag("InputController", false);

   //this->full_logger_.file_start("log.txt");
   //this->full_logger_.get_logger("file")->set_tag("Viewport", false);
   //this->full_logger_.get_logger("file")->set_tag("InputController", false);

   Service::provide_logger(&this->full_logger_);
   
   // set up input
   Service::provide_input(&this->input_);
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this));
   
   // load fonts
   this->graphics.load_font("retro", "retro.ttf");
}

Game::~Game() {
   this->reset();
}

void Game::start() {
   assert(!this->is_running_); // start should only be called once (or unless reset is called)

   this->is_running_ = true;
   this->main_loop();
}

void Game::reset() {
   // remove all scenes
   while (!this->scenes_.empty()) {
      delete this->scenes_.top();
      this->scenes_.pop();
   }
   this->is_running_ = false;
}

void Game::load_scene(Scene* scene) {
   assert(!this->next_scene_);
   this->next_scene_ = scene;
}

void Game::unload_scene() {
   assert(!this->prev_scene_);
   this->prev_scene_ = this->scenes_.top();
   this->scenes_.pop();
}

Scene* Game::switch_scene(Scene* scene) {
   this->unload_scene();
   this->load_scene(scene);
   return this->prev_scene_;
}

void Game::process(CloseCommand& c) {
   this->scenes_.top()->process(*this, c);
}

void Game::process(KeyPressCommand& c) {
   this->scenes_.top()->process(*this, c);
}

void Game::process(WindowResizeCommand& c) {
   this->scenes_.top()->process(*this, c);
}

void Game::process(MouseMoveCommand& c) {
   this->scenes_.top()->process(*this, c);
}

void Game::process(MouseButtonCommand& c) {
   this->scenes_.top()->process(*this, c);
}

void Game::process(MouseWheelCommand& c) {
   this->scenes_.top()->process(*this, c);
}

void Game::main_loop() {
   while (this->graphics.is_open()) {
      // poll input
      Service::get_input().pollEvents(*this);

      // clear graphics
      this->graphics.clear();
   
      if (this->scenes_.empty() && !this->next_scene_) {
         Service::get_logger().msg("Game", Logger::INFO, "Exiting game loop...");
         this->exit();
         return;
      }

      // check if we need to unload a scene
      if (this->prev_scene_) {
         this->prev_scene_->exit(*this);
         delete this->prev_scene_;
         this->prev_scene_ = nullptr;
         continue; // do empty check again
      }

      // check if we need to load a scene
      if (this->next_scene_) {
         this->next_scene_->enter(*this);
         this->scenes_.push(this->next_scene_);
         this->next_scene_ = nullptr;
      }

      // update
      this->scenes_.top()->update(*this);
      this->graphics.update();
   }
}

void Game::exit() {
   this->graphics.close();
   this->reset();
}
