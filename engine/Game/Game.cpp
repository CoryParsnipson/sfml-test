#include <cassert>

#include "Game.h"
#include "TextFactory.h"

#include "FullLogger.h"

#include "FileChannel.h"
#include "JSONSerializer.h"

#include "Mouse.h"
#include "Keyboard.h"

#include "CloseInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "KeyReleaseInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

#include "Scene.h"

// ----------------------------------------------------------------------------
// static member initialization
// ----------------------------------------------------------------------------
Logger* Game::logger_ = new FullLogger();

Game::Game()
: InputListener("Game")
, input_manager_(new InputManager())
, next_scene_(nullptr)
, prev_scene_(nullptr)
, window_("SFML Test", sf::Vector2f(this->settings.default_window_width(), this->settings.default_window_height()))
{
   // TODO: probably need to move this into a config class or something
   Channel* config_file = new FileChannel("config.txt");
   Serializer* config_reader = new JSONSerializer();

   FullLogger* logger = dynamic_cast<FullLogger*>(&Game::logger());

   Scene* dummy_scene = new Scene("DummyScene");
   std::string raw_config_line = config_reader->read(*config_file);
   while (!raw_config_line.empty()) {
      Serializer::SerialData config_line = config_reader->deserialize(*dummy_scene, raw_config_line);

      if (config_line["type"] == "logger") {
         if (config_line["tag"] == "enable") {
            if (config_line["stream"] == "console") {
               logger->console_start();
            } else if (config_line["stream"] == "file") {
               logger->file_start("log.txt");
            }
         } else if (config_line["tag"] == "disable_all_tags") {
            logger->get_logger(config_line["stream"])->disable_all_tags();
         } else {
            bool setting = (config_line["enable"] == "true");
            logger->get_logger(config_line["stream"])->set_tag(config_line["tag"], setting);
         }
      }

      raw_config_line = config_reader->read(*config_file);
   }

   delete dummy_scene;
   delete config_file;
   delete config_reader;

   // set up input
   this->input_manager().attach(*this);

   this->input_manager().add_device(new Mouse());
   this->input_manager().add_device(new Keyboard());
}

Game::~Game() {
   this->reset();

   // remove from input
   this->input_manager().detach(*this);

   // destroy player profiles
   for (Players::const_iterator it = this->players_.begin(); it != this->players_.end(); ++it) {
      delete it->second;
   }
   this->players_.clear();

   // destroy logger
   delete Game::logger_;
}

InputManager& Game::input_manager() {
   return *this->input_manager_;
}

Logger& Game::logger() {
   return *Game::logger_;
}

Player& Game::add_player(Player::PlayerId id) {
   Players::iterator it = this->players_.find(id);
   if (it == this->players_.end()) {
      this->players_[id] = new Player(id);
   }

   return *(this->players_[id]);
}

Player& Game::get_player(Player::PlayerId id) {
   return *(this->players_.at(id));
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
   this->logger().msg("Game", Logger::INFO, "Loading scene '" + scene->id() + "'");

   assert(!this->next_scene_);
   this->next_scene_ = scene;
}

void Game::unload_scene() {
   assert(!this->prev_scene_);
   this->prev_scene_ = this->scenes_.top();
   this->logger().msg("Game", Logger::INFO, "Unloading scene '" + this->prev_scene_->id() + "'");
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

void Game::process(KeyReleaseInputEvent& e) {
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

Canvas& Game::window() {
   return this->window_;
}

void Game::main_loop() {
   while (true) {
      if (this->scenes_.empty() && !this->prev_scene_ && !this->next_scene_) {
         this->logger().msg("Game", Logger::INFO, "Exiting game loop...");
         return;
      }

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

         // initialize scene if this is the first time we're loading it
         if (!this->next_scene_->is_initialized()) {
            this->next_scene_->do_init(*this);
         }

         this->next_scene_->do_enter(*this);
         this->next_scene_ = nullptr;
      }

      if (this->scenes_.empty()) {
         continue;
      }

      // ------------
      // Need to change these lines to see new ECS system updates

      // draw
      this->window_.clear(); // clear previous frame contents

      // update
      this->scenes_.top()->update(*this);

      // ------------

// TODO: uncomment this for old behavior
//      // update
//      this->scenes_.top()->update(*this);
//
//      // draw
//      this->window_.clear(); // clear previous frame contents
//      this->scenes_.top()->draw(this->window_);

      // poll input
      this->input_manager().poll_event(*this);

      // render scene
      this->window_.update();
   }
}
