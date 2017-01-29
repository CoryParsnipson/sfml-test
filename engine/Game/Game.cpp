#include <cassert>

#include "Game.h"
#include "TextFactory.h"

#include "FileChannel.h"
#include "JSONSerializer.h"

#include "CloseInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

#include "Scene.h"

Game::Game()
: InputListener("Game")
, next_scene_(nullptr)
, prev_scene_(nullptr)
, window_("SFML Test", sf::Vector2f(this->settings.default_window_width(), this->settings.default_window_height()))
{
   // TODO: probably need to move this into a config class or something
   Channel* config_file = new FileChannel("config.txt");
   Serializer* config_reader = new JSONSerializer();

   Scene* dummy_scene = new Scene("DummyScene");
   std::string raw_config_line = config_reader->read(*config_file);
   while (!raw_config_line.empty()) {
      Serializer::SerialData config_line = config_reader->deserialize(*dummy_scene, raw_config_line);

      if (config_line["type"] == "logger") {
         if (config_line["tag"] == "enable") {
            if (config_line["stream"] == "console") {
               this->full_logger_.console_start();
            } else if (config_line["stream"] == "file") {
               this->full_logger_.file_start("log.txt");
            }
         } else if (config_line["tag"] == "disable_all_tags") {
            this->full_logger_.get_logger(config_line["stream"])->disable_all_tags();
         } else {
            bool setting = (config_line["enable"] == "true");
            this->full_logger_.get_logger(config_line["stream"])->set_tag(config_line["tag"], setting);
         }
      }

      raw_config_line = config_reader->read(*config_file);
   }

   delete dummy_scene;
   delete config_file;
   delete config_reader;

   Service::provide_logger(&this->full_logger_);

   // set up input
   Service::provide_input(&this->input_);
   Service::get_input().attach(*this);
}

Game::~Game() {
   this->reset();
}

void Game::start() {
   this->main_loop();
}

void Game::reset() {
   // remove all scenes
   while (!this->scenes_.empty()) {
      delete this->scenes_.top();
      this->scenes_.pop();
   }
}

void Game::load_scene(Scene* scene) {
   Service::get_logger().msg("Game", Logger::INFO, "Loading scene '" + scene->id() + "'");

   assert(!this->next_scene_);
   this->next_scene_ = scene;
}

void Game::unload_scene() {
   assert(!this->prev_scene_);
   this->prev_scene_ = this->scenes_.top();
   Service::get_logger().msg("Game", Logger::INFO, "Unloading scene '" + this->prev_scene_->id() + "'");
}

Scene* Game::switch_scene(Scene* scene) {
   this->unload_scene();
   this->load_scene(scene);
   return this->prev_scene_;
}

Scene* Game::current_scene() {
   return (this->scenes_.empty() ? nullptr : this->scenes_.top());
}

void Game::process(CloseInputEvent& e) {
   this->scenes_.top()->process(*this, e);
}

void Game::process(ResizeInputEvent& e) {
   this->scenes_.top()->process(*this, e);
}

void Game::process(KeyPressInputEvent& e) {
   this->scenes_.top()->process(*this, e);
}

void Game::process(MouseMoveInputEvent& e) {
   this->scenes_.top()->process(*this, e);
}

void Game::process(MouseWheelInputEvent& e) {
   this->scenes_.top()->process(*this, e);
}

void Game::process(MouseButtonInputEvent& e) {
   this->scenes_.top()->process(*this, e);
}

bool Game::poll_event(sf::Event& event) {
   return this->window_.poll_event(event);
}

const Canvas& Game::window() const {
   return this->window_;
}

void Game::main_loop() {
   while (true) {
      if (this->scenes_.empty() && !this->prev_scene_ && !this->next_scene_) {
         Service::get_logger().msg("Game", Logger::INFO, "Exiting game loop...");
         return;
      }

      // poll input
      Service::get_input().poll_event(*this);

      // check if we need to unload a scene
      if (this->prev_scene_) {
         this->prev_scene_->do_exit(*this);

         assert(this->prev_scene_ = this->scenes_.top());
         this->scenes_.pop();

         delete this->prev_scene_;
         this->prev_scene_ = nullptr;
      }

      // check if we need to load a scene
      if (this->next_scene_) {
         this->scenes_.push(this->next_scene_);
         this->next_scene_->do_enter(*this);
         this->next_scene_ = nullptr;
      }

      if (this->scenes_.empty()) {
         continue;
      }

      // update
      this->scenes_.top()->update(*this);

      // draw
      this->window_.clear(); // clear previous frame contents
      this->scenes_.top()->draw(this->window_);

      // render scene
      this->window_.update();
   }
}
