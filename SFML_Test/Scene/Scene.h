#ifndef SCENE_H
#define SCENE_H

#include "dependencies.h"

#include "CloseInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

#include "Game.h"
#include "Draw.h"
#include "Camera.h"
#include "Update.h"
#include "Entity.h"
#include "Widget.h"
#include "Gamepad.h"
#include "FontDictionary.h"

#include "RemoveCommand.h"

class Scene
: public Draw
, public Update
{
public:
   using GamepadList = std::vector<Gamepad*>;

   Scene(std::string id)
   : id_(id)
   , camera_(new Camera("Camera", sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height())))
   , scene_graph_(this->camera_)
   {
      Service::get_logger().msg(this->id(), Logger::INFO, "Creating new Scene.");
   }

   virtual ~Scene() {
      Service::get_logger().msg(this->id(), Logger::INFO, "Destroying Scene.");

      // TODO: need to properly clean up scene objects
      delete this->scene_graph_;

      GamepadList::const_iterator g_it;
      for (g_it = this->gamepads_.begin(); g_it != this->gamepads_.end(); ++g_it) {
         delete *g_it;
      }
      this->gamepads_.clear();
      
      delete this->game_;
   }

   virtual std::string id() { return this->id_; }

   void do_enter(Game& game) {
      // start gamepads from receiving input events
      for (GamepadList::const_iterator it = this->gamepads_.begin(); it != this->gamepads_.end(); ++it) {
         Service::get_input().attach(**it);
      }

      this->game_ = &game; // set game pointer
      this->enter(game);
   }

   void do_exit(Game& game) {
      // stop gamepads from receiving input events
      for (GamepadList::const_iterator it = this->gamepads_.begin(); it != this->gamepads_.end(); ++it) {
         Service::get_input().detach(**it);
      }

      this->game_ = nullptr; // unset game pointer
      this->exit(game);
   }

   virtual void enter(Game& game) {}
   virtual void exit(Game& game) {}

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {
      SceneObject::prefix_iterator it;
      for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
         (*it)->draw(surface, render_states);
      }
   }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr) {
      SceneObject::prefix_iterator it;
      for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
         (*it)->update(game, scene);
      }
   }

   // gamepad interface
   int gamepad(Gamepad* gamepad, int player_id = -1) {
      if (player_id >= 0 && player_id < (int)this->gamepads_.size()) {
         // remove gamepad scene object from scene graph
         RemoveCommand rc(this->scene_graph_, gamepad);
         rc.execute();

         // remove gamepad from gamepad list and replace
         delete this->gamepads_[player_id];
         this->gamepads_[player_id] = gamepad;

         return player_id;
      } else {
         this->gamepads_.push_back(gamepad);    // add to gamepad list
         this->scene_graph_->add(gamepad);      // add to scene graph

         return this->gamepads_.size() - 1;
      }
   }

   Gamepad* gamepad(int player_id) {
      if (player_id < 1 || player_id > (signed int)this->gamepads_.size()) {
         return this->gamepads_[player_id + 1];
      }
      return nullptr;
   }

   // TODO: implement
   void remove_gamepad(int player_id);
   void remove_gamepad(Gamepad* gamepad);

   // scene interface
   void load_scene(Scene* scene) {
      if (!this->game_) {
         Service::get_logger().msg(this->id(), Logger::ERROR, "Cannot load scene from a scene that is not on top of the game stack.");
         return;
      }

      this->game_->load_scene(scene);
   }

   Scene* switch_scene(Scene* scene) {
      if (!this->game_) {
         Service::get_logger().msg(this->id(), Logger::ERROR, "Cannot switch scene from a scene that is not on top of the game stack.");
         return nullptr;
      }

      return this->game_->switch_scene(scene);
   }

   // input event processing default implementations
   virtual void process(Game& game, CloseInputEvent& e) {
      game.unload_scene();
   }
   virtual void process(Game& game, KeyPressInputEvent& e) {}
   virtual void process(Game& game, ResizeInputEvent& e) {
      // update main camera
      this->camera_->set_size(sf::Vector2f(e.width, e.height));
      this->camera_->set_center(sf::Vector2f(e.width / 2.f, e.height / 2.f));
   }
   virtual void process(Game& game, MouseMoveInputEvent& e) {}
   virtual void process(Game& game, MouseWheelInputEvent& e) {}
   virtual void process(Game& game, MouseButtonInputEvent& e) {}

protected:
   std::string id_;
   Camera* camera_;
   SceneObject* scene_graph_;

   FontDictionary fonts_;
   Dictionary<std::string, Texture> textures_;

private:
   Game* game_;
   GamepadList gamepads_;
};

#endif
