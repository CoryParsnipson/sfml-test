#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include "sfml.h"

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
#include "FontAtlas.h"
#include "TextureAtlas.h"

#include "SceneRenderer.h"

#include "RemoveCommand.h"
#include "ResizeCameraCommand.h"

class Scene
: public Draw
, public Update
{
public:
   using GamepadList = std::vector<Gamepad*>;

   Scene(std::string id)
   : id_(id)
   , camera_(new Camera("Camera"))
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

      // resize all the cameras
      ResizeCameraCommand rc_command(game.window(), this->scene_graph_);
      rc_command.execute();

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
      this->renderer_.render(*this->scene_graph_, surface, render_states);
   }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr) {
      SceneObject::prefix_iterator it;
      for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
         (*it)->update(game, (scene ? scene : this));
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

   void remove_gamepad(int player_id) {
      if (player_id >= (signed int)this->gamepads_.size()) {
         return;
      }
      
      delete this->gamepads_[player_id];
      this->gamepads_.erase(this->gamepads_.begin() + player_id);
   }

   void remove_gamepad(Gamepad* gamepad) {
      GamepadList::const_iterator it;
      for (it = this->gamepads_.begin(); it != this->gamepads_.end(); ++it) {
         if (gamepad == *it) {
            delete gamepad;
            this->gamepads_.erase(it);
            return;
         }
      }
   }

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

   Scene* unload_scene() {
      if (!this->game_) {
         Service::get_logger().msg(this->id(), Logger::ERROR, "Cannot unload scene that is not on top of the game stack.");
         return nullptr;
      }

      this->game_->unload_scene();
      return this->game_->current_scene();
   }

   SceneObject* scene_graph() {
      return this->scene_graph_;
   }

   // input event processing default implementations
   virtual void process(Game& game, CloseInputEvent& e) {
      game.unload_scene();
   }
   virtual void process(Game& game, KeyPressInputEvent& e) {}
   virtual void process(Game& game, ResizeInputEvent& e) {
      // resize all the cameras
      ResizeCameraCommand rc_command(game.window(), this->scene_graph_);
      rc_command.execute();
   }
   virtual void process(Game& game, MouseMoveInputEvent& e) {}
   virtual void process(Game& game, MouseWheelInputEvent& e) {}
   virtual void process(Game& game, MouseButtonInputEvent& e) {}

protected:
   std::string id_;
   Camera* camera_;
   SceneObject* scene_graph_;

   FontAtlas fonts_;
   TextureAtlas textures_;

private:
   Game* game_;
   GamepadList gamepads_;
   SceneRenderer renderer_;
};

#endif