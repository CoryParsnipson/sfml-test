#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <memory>
#include <cassert>

#include "CloseInputEvent.h"
#include "LostFocusInputEvent.h"
#include "GainedFocusInputEvent.h"
#include "MouseEnteredInputEvent.h"
#include "MouseLeftInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "KeyReleaseInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

#include "Game.h"
#include "Camera.h"
#include "Update.h"
#include "Entity.h"
#include "FontAtlas.h"
#include "TextureAtlas.h"

#include "Space.h"

#include "ComponentManager.h"

#include "System.h"
#include "Mailbox.h"

#include "SpatialSystem.h"
#include "CallbackSystem.h"
#include "GraphicalSystem.h"

#include "EntityCreatedMessage.h"
#include "AddToEntityMessage.h"
#include "ResizeCameraMessage.h"

// ----------------------------------------------------------------------------
// Scene
//
// This is the "world" that all entities, systems, and components live in.
// This is a discrete container to organize game play and "levels" with.
// ----------------------------------------------------------------------------
class Scene
: public Update
{
public:
   Scene(std::string id)
   : id_(id)
   , is_initialized_(false)
   , entities_(id + "EntityPool", 20000, [this](){ return Entity("entity", this, &this->components_); })
   {
   }

   virtual ~Scene() {
      // clean up Systems
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         delete *it;
      }
      this->systems_.clear();
   }

   virtual std::string id() { return this->id_; }
   bool is_initialized() const { return this->is_initialized_; }

   void do_init(Game& game) {
      this->game_ = &game; // set game pointer

      // create scene graph root entity
      this->root_ = this->create_entity();
      this->get_entity(this->root_)->id(this->id() + "RootEntity");
      this->get_entity(this->root_)->get<Space>()->id("RootSpaceComponent");

      // add some default systems
      this->add_system(new SpatialSystem());
      this->add_system(new CallbackSystem());
      this->add_system(new GraphicalSystem("GraphicalSystem", game.window(), std::make_shared<Camera>("Default Camera")));

      this->init(game);
      this->is_initialized_ = true;
   }

   void do_enter(Game& game) {
      // tell all graphical based systems to resize their cameras
      this->send_message_async<ResizeCameraMessage>(game.window().size());

      this->enter(game);
   }

   void do_exit(Game& game) {
      this->game_ = nullptr; // unset game pointer
      this->exit(game);
   }

   virtual void init(Game& game) {}
   virtual void enter(Game& game) {}
   virtual void exit(Game& game) {}

   // update interface
   virtual void update(Game& game) {
      // update systems
      for (std::vector<System*>::iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         (*it)->update(game);
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
      Handle handle = this->entities_.add("entity", this, &this->components_);
      assert(this->get_entity(handle) != nullptr);

      // set the Entity's handle
      this->get_entity(handle)->handle(handle);

      // let Systems know a new Entity has been made
      this->send_message_async<EntityCreatedMessage>(handle);

      // add Spatial component, every Entity should always have one
      this->get_entity(handle)->add<Space>();

      // now add this entity as a child of the scene graph root
      if (this->root_ != Handle() && this->root_ != handle) {
        this->send_message_async<AddToEntityMessage>(this->root_, handle);
      }

      return handle;
   }

   Entity* get_entity(Handle handle) {
      return this->entities_.get(handle);
   }

   void remove_entity(Handle handle) {
      this->entities_.remove(handle);
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

   System* get_system(std::string id) const {
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         if ((*it)->id() == id) {
            return *it;
         }
      }

      return nullptr;
   }

   // input event processing default implementations
   virtual void process(Game& game, CloseInputEvent& e) {
      game.unload_scene();
   }
   virtual void process(Game& game, LostFocusInputEvent& e) {}
   virtual void process(Game& game, GainedFocusInputEvent& e) {}
   virtual void process(Game& game, MouseEnteredInputEvent& e) {}
   virtual void process(Game& game, MouseLeftInputEvent& e) {}
   virtual void process(Game& game, KeyPressInputEvent& e) {}
   virtual void process(Game& game, KeyReleaseInputEvent& e) {}
   virtual void process(Game& game, ResizeInputEvent& e) {
      // resize all the cameras
      this->send_message_async<ResizeCameraMessage>(game.window().size());
   }
   virtual void process(Game& game, MouseMoveInputEvent& e) {}
   virtual void process(Game& game, MouseWheelInputEvent& e) {}
   virtual void process(Game& game, MouseButtonInputEvent& e) {}

   Space& space() {
      assert(this->get_entity(this->root_));
      assert(this->get_entity(this->root_)->get<Space>());

      return *this->get_entity(this->root_)->get<Space>();
   }

   Handle space_handle() {
      return this->root_;
   }

   // broadcast a message to all systems in the scene; this method is called by System
   void handle_message(std::shared_ptr<Message> msg) {
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         (*it)->receive_message(msg);
      }
   }

protected:
   // use this to broadcast messages to Systems from Scene
   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
      typename... Args
   >
   void send_message(Args&&... args);

   template <
      typename MsgT,
      typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
      typename... Args
   >
   void send_message_async(Args&&... args);

private:
   std::string id_;
   bool is_initialized_;

   Game* game_;

   FontAtlas fonts_;
   TextureAtlas textures_;

   ObjectPool<Entity> entities_;
   ComponentManager components_;
   std::vector<System*> systems_;

   Handle root_;
};

// ----------------------------------------------------------------------------
// template member declarations
// ----------------------------------------------------------------------------
template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void Scene::send_message(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);
   this->handle_message(message);
}

template <
   typename MsgT,
   typename std::enable_if<std::is_base_of<Message, MsgT>::value>::type* = nullptr,
   typename... Args
>
void Scene::send_message_async(Args&&... args) {
   std::shared_ptr<MsgT> message = std::make_shared<MsgT>(std::forward<Args>(args)...);
   message->async(true);

   this->handle_message(message);
}

#endif
