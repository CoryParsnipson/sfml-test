#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <memory>
#include <cassert>

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

#include "ComponentManager.h"

#include "System.h"
#include "Mailbox.h"

// ----------------------------------------------------------------------------
// Scene
//
// This is the "world" that all entities, systems, and components live in.
// This is a discrete container to organize game play and "levels" with.
// ----------------------------------------------------------------------------
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
   , entities_(id + "EntityPool", 20000, [this](){ return Entity("entity", &this->components_); })
   {
   }

   virtual ~Scene() {
      // TODO: need to properly clean up scene objects
      delete this->scene_graph_;

      GamepadList::const_iterator g_it;
      for (g_it = this->gamepads_.begin(); g_it != this->gamepads_.end(); ++g_it) {
         delete *g_it;
      }
      this->gamepads_.clear();

      // clean up Systems
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         delete *it;
      }
      this->systems_.clear();
   }

   virtual std::string id() { return this->id_; }

   void do_enter(Game& game) {
      // resize all the cameras
      ResizeCameraCommand rc_command(game.window(), this->scene_graph_);
      rc_command.execute();

      this->game_ = &game; // set game pointer
      this->enter(game);

      // start gamepads from receiving input events
      for (GamepadList::const_iterator it = this->gamepads_.begin(); it != this->gamepads_.end(); ++it) {
        game.input().attach(**it);
      }
   }

   void do_exit(Game& game) {
      // stop gamepads from receiving input events
      for (GamepadList::const_iterator it = this->gamepads_.begin(); it != this->gamepads_.end(); ++it) {
         game.input().detach(**it);
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
   virtual void update(Game& game) {
      SceneObject::prefix_iterator it;
      for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
         (*it)->update(game);
      }

      // update systems
      for (std::vector<System*>::iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         (*it)->update(game);
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
      } else {
         player_id = this->gamepads_.size() - 1;
         this->gamepads_.push_back(gamepad);
      }

      // add to scene graph
      this->scene_graph_->add(gamepad);
      return player_id;
   }

   Gamepad* gamepad(int player_id) {
      if (player_id < 0 || player_id >= (signed int)this->gamepads_.size()) {
         return nullptr;
      }
      return this->gamepads_[player_id];
   }

   void remove_gamepad(int player_id) {
      if (player_id > (signed int)this->gamepads_.size()) {
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
      assert(this->game_); // can only load scene from a scene on the top of the game stack
      this->game_->load_scene(scene);
   }

   Scene* switch_scene(Scene* scene) {
      assert(this->game_); // can only switch scene from a scene on the top of the game stack
      return this->game_->switch_scene(scene);
   }

   Scene* unload_scene() {
      assert(this->game_); // can only unload scene from a scene on the top of the game stack
      this->game_->unload_scene();
      return this->game_->current_scene();
   }

   SceneObject* scene_graph() {
      return this->scene_graph_;
   }

   FontAtlas& fonts() {
      return this->fonts_;
   }

   TextureAtlas& textures() {
      return this->textures_;
   }

   Game& game() {
      assert(this->game_); // should only be called when this is the top scene
      return *this->game_;
   }

   // entity component system interface
   Handle create_entity() {
      return this->entities_.add();
   }

   Entity* get_entity(Handle handle) {
      return this->entities_.get(handle);
   }
   
   std::vector<Handle> entities() const {
      return this->entities_.get_active_handles();
   }

   void add_system(System* system, int priority = -1) {
      assert(this->game_ != nullptr);

      if (!system) {
         this->game_->logger().msg(this->id(), Logger::WARNING, "In add_system(), ignoring nullptr system argument.");
         return;
      }

      // don't forget to initialize system
      system->init(*this->game_);

      if (priority < 0 || static_cast<unsigned int>(priority) >= this->systems_.size()) {
         this->systems_.push_back(system);
         this->game_->logger().msg(this->id(), Logger::INFO, "Adding system '" + system->id() + "' to end of systems vector.");
      } else {
         this->systems_.insert(this->systems_.begin() + priority, system);
         this->game_->logger().msg(this->id(), Logger::INFO, "Adding system '" + system->id() + "' to position " + std::to_string(priority) + " of systems vector.");
      }
   }

   void handle_message(std::shared_ptr<Message> msg) {
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         (*it)->receive_message(msg);
      }
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

   const SceneObject& scene_graph() const {
      return *this->scene_graph_;
   }

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

   ObjectPool<Entity> entities_;
   ComponentManager components_;
   std::vector<System*> systems_;
};

#endif
