#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <stdexcept>
#include <typeindex>

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
#include "Updateable.h"
#include "Camera.h"
#include "Entity.h"
#include "FontAtlas.h"
#include "TextureAtlas.h"

#include "SceneNode.h"

#include "ComponentManager.h"

#include "System.h"
#include "Messageable.h"

#include "CallbackSystem.h"
#include "GraphicalSystem.h"

#include "EntityCreatedMessage.h"
#include "EntityDestroyedMessage.h"
#include "ResizeCameraMessage.h"
#include "EntityIdChangedMessage.h"

#include "Font.h"

// ----------------------------------------------------------------------------
// Scene
//
// This is the "world" that all entities, systems, and components live in.
// This is a discrete container to organize game play and "levels" with.
// ----------------------------------------------------------------------------
class Scene
: public Updateable
, public Messageable
{
public:
   using SystemHash = std::map<std::type_index, std::map<std::string, System*>>;

   Scene(std::string id)
   : Messageable(id)
   , id_(id)
   , is_initialized_(false)
   , entities_(id + "EntityPool", 20000, Entity("entity", this, &this->components_))
   , root_()
   , scene_graph_(nullptr)
   {
   }

   virtual ~Scene() {
      // clean up Systems
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         delete *it;
      }
      this->systems_.clear();

      for (SystemHash::iterator it = this->system_hash_.begin(); it != this->system_hash_.end(); ++it) {
         it->second.clear();
      }
      this->system_hash_.clear();

      // destroy scene graph
      if (this->scene_graph()) {
         std::vector<SceneNode*> graph_nodes;
         graph_nodes.push_back(this->scene_graph());

         while (!graph_nodes.empty()) {
            SceneNode* current = graph_nodes.front();
            graph_nodes.erase(graph_nodes.begin());

            for (unsigned int i = 0; i < current->num_children(); ++i) {
               graph_nodes.push_back(current->get_child(i));
            }
            delete current;
         }
      }
   }

   virtual std::string id() { return this->id_; }
   bool is_initialized() const { return this->is_initialized_; }

   void do_init(Game& game) {
      this->game_ = &game; // set game pointer

      // create scene graph root entity
      assert(this->root_ == Handle() && !this->scene_graph_);
      this->create_entity_handle(this->id() + "RootEntity");

      // add some default systems
      this->add_system(new CallbackSystem());
      this->add_system(new GraphicalSystem("GraphicalSystem", game.window(), std::make_shared<Camera>("Default Camera")));

      // provide default actions for handling certain messages
      this->install_message_handler<EntityIdChangedMessage>([this](EntityIdChangedMessage& msg) {
         // if the ID changes, we need to update the bookmarks
         this->remove_bookmark(msg.old_id);
         this->bookmark(msg.entity);
      });

      this->init(game);
      this->is_initialized_ = true;
   }

   void do_enter(Game& game) {
      // tell all graphical based systems to resize their cameras
      this->send_message<ResizeCameraMessage>(game.window().size());

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
      // handle any synchronous messages for the Scene
      this->handle_queued_messages();

      // handle any synchronous messages received by entities
      std::vector<Handle> entities = this->entities();
      for (std::vector<Handle>::iterator it = entities.begin(); it != entities.end(); ++it) {
         Entity* e = this->get_entity(*it);
         if (e) {
            e->handle_queued_messages();
         }
      }

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
   Entity* create_entity(const std::string& id = "") {
      Handle handle = this->entities_.add(id, this, &this->components_);
      assert(this->get_entity(handle) != nullptr);

      // set the Entity's handle
      Entity* e = this->get_entity(handle);
      e->handle(handle);

      if (id == "") {
         std::string auto_id = "Entity" + std::to_string(handle.index());
         e->id(auto_id);
      }
      this->bookmark(e);
      
      // let Systems know a new Entity has been made
      this->send_message<EntityCreatedMessage>(handle);

      // add scene graph node
      SceneNode* node = new SceneNode();
      node->entity(handle);
      e->space(node);

      // if we aren't creating the root node, automatically add this entity as a child of the root
      if (this->root_ != Handle()) {
         assert(this->scene_graph_);
         this->add_to_scene_node(this->root_entity(), e);
      } else {
         this->root_ = handle;
         this->scene_graph_ = node;
      }
      
      return e;
   }

   Handle create_entity_handle(const std::string& id = "") {
      return this->create_entity(id)->handle();
   }

   Entity* get_entity(Handle handle) {
      return this->entities_.get(handle);
   }

   Entity* get_entity(const std::string& id) {
      return this->get_entity(this->bookmarks_[id]);
   }

   Handle get_entity_handle(const std::string& id) {
      return this->bookmarks_[id];
   }

   void remove_entity(Handle handle, bool recursive = false) {
      Entity* e = this->get_entity(handle);
      if (!e) {
         // fail silently
         return;
      }

      // remove scene graph node
      SceneNode* space = e->space();
      assert(space->entity() == handle);

      int parent_idx = -1;
      SceneNode* parent = space->parent();
      assert (parent); // don't allow deletion of root node

      for (unsigned int i = 0; i < parent->num_children(); ++i) {
         if (space == parent->get_child(i)) {
            parent_idx = i;
            break;
         }
      }
      parent->remove(parent_idx);

      if (!recursive) {
         // let Systems know a new Entity has been deallocated
         this->send_message<EntityDestroyedMessage>(handle);

         for (unsigned int i = 0; i < space->num_children(); ++i) {
            space->parent()->add(space->get_child(i), parent_idx + i);
            space->get_child(i)->parent(space->parent());
         }
         delete space;

         this->entities_.get(handle)->reset();
         this->entities_.remove(handle);
      } else {
         // if recursive is set, delete all children of handle too
         std::vector<Handle> entities;
         std::vector<Handle> postorder;
         std::vector<SceneNode*> nodes;

         entities.push_back(handle);

         while (!entities.empty()) {
            Entity* current = this->get_entity(entities.back());
            entities.pop_back();

            if (!current) {
               continue;
            }

            postorder.push_back(current->handle());
            nodes.push_back(current->space());

            for (unsigned int i = 0; i < current->space()->num_children(); ++i) {
               entities.push_back(current->space()->get_child(i)->entity());
            }
         }

         for (std::vector<Handle>::reverse_iterator it = postorder.rbegin(); it != postorder.rend(); ++it) {
            // let Systems know a new Entity has been deallocated
            this->send_message<EntityDestroyedMessage>(*it);

            this->entities_.get(*it)->reset();
            this->entities_.remove(*it);
         }

         for (std::vector<SceneNode*>::reverse_iterator it = nodes.rbegin(); it != nodes.rend(); ++it) {
            delete *it;
         }
      }
   }

   void add_to_scene_node(Handle parent, Handle child, int idx = -1) {
      Entity* p = this->get_entity(parent);
      Entity* c = this->get_entity(child);
      add_to_scene_node(p->space(), c->space(), idx);
   }

   void add_to_scene_node(Entity* parent, Handle child, int idx = -1) {
      Entity* c = this->get_entity(child);
      add_to_scene_node(parent->space(), c->space(), idx);
   }

   void add_to_scene_node(Handle parent, Entity* child, int idx = -1) {
      Entity* p = this->get_entity(parent);
      add_to_scene_node(p->space(), child->space(), idx);
   }

   void add_to_scene_node(Entity* parent, Entity* child, int idx = -1) {
      add_to_scene_node(parent->space(), child->space(), idx);
   }

   void add_to_scene_node(SceneNode* parent, Entity* child, int idx = -1) {
      add_to_scene_node(parent, child->space(), idx);
   }

   void add_to_scene_node(SceneNode* parent, SceneNode* child, int idx = -1) {
      if (!parent || !child) {
         this->game_->logger().msg(this->id(), Logger::WARNING, "add_to_scene_node() received nullptr to entity. Ignoring.");
         return;
      }

      if (child->parent() && child->parent()->entity() == parent->entity()) {
         parent->remove(child);
      }

      parent->add(child, idx);
      if (child->parent()) {
         child->parent()->remove(child);
      }
      child->parent(parent);
   }
   
   std::vector<Handle> entities() const {
      return this->entities_.get_active_handles();
   }

   Handle root_entity() const {
      return this->root_;
   }

   SceneNode* scene_graph() const {
      return this->scene_graph_;
   }

   sf::Transform global_transform(Entity& e) {
      sf::Transform g_transform = sf::Transform();
      SceneNode* space = e.space();

      while (space != nullptr) {
         g_transform *= space->transform();
         space = space->parent();
      }

      return g_transform;
   }

   template <
      typename SystemType,
      typename std::enable_if<std::is_base_of<System, SystemType>::value>::type* = nullptr
   >
   void add_system(SystemType* system, int priority = -1) {
      assert(this->game_ != nullptr);

      if (!system) {
         this->game_->logger().msg(this->id(), Logger::WARNING, "In add_system(), ignoring nullptr system argument.");
         return;
      }

      // do unique string id check
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         if ((*it)->id() == system->id()) {
            throw std::range_error("Cannot add system of type '" + std::string(typeid(SystemType).name()) + "': id '" + system->id() + "' already in use.");
         }
      }

      // don't forget to initialize system
      system->init(*this->game_);

      // add to systems hash
      this->system_hash_[std::type_index(typeid(SystemType))][system->id()] = system;

      // add to priority list
      if (priority < 0 || static_cast<unsigned int>(priority) >= this->systems_.size()) {
         this->systems_.push_back(system);
         this->game_->logger().msg(this->id(), Logger::INFO, "Adding system '" + system->id() + "' to end of systems vector.");
      } else {
         this->systems_.insert(this->systems_.begin() + priority, system);
         this->game_->logger().msg(this->id(), Logger::INFO, "Adding system '" + system->id() + "' to position " + std::to_string(priority) + " of systems vector.");
      }

   }

   template <
      typename SystemType,
      typename std::enable_if<std::is_base_of<System, SystemType>::value>::type* = nullptr
   >
   void remove_system(std::string id) {
      SystemHash::iterator it = this->system_hash_.find(std::type_index(typeid(SystemType)));
      if (it == this->system_hash_.end() || it->second.find(id) == it->second.end()) {
         return;
      }
      it->second.erase(id);

      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         if ((*it)->id() == id) {
            this->systems_.erase(it);
            break;
         }
      }
   }

   template <
      typename SystemType,
      typename std::enable_if<std::is_base_of<System, SystemType>::value>::type* = nullptr
   >
   SystemType* get_system(std::string id) const {
      SystemHash::const_iterator it = this->system_hash_.find(std::type_index(typeid(SystemType)));
      if (it != this->system_hash_.end() && it->second.find(id) != it->second.end()) {
         return static_cast<SystemType*>(it->second.at(id));
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
      this->send_message<ResizeCameraMessage>(game.window().size());
   }
   virtual void process(Game& game, MouseMoveInputEvent& e) {}
   virtual void process(Game& game, MouseWheelInputEvent& e) {}
   virtual void process(Game& game, MouseButtonInputEvent& e) {}

protected:
   // store a handle to an entity with a string identifier
   void bookmark(const std::string& bookmark_id, Handle entity) {
      if (this->get_entity(bookmark_id) != nullptr) {
         throw std::runtime_error("Entity ID '" + bookmark_id + "' is already in use!");
      }

      this->bookmarks_[bookmark_id] = entity;
   }

   void bookmark(Handle entity) {
      Entity* e = this->get_entity(entity);
      if (!e) {
         return; // fails silently
      }

      this->bookmark(e->id(), entity);
   }

   void bookmark(Entity* entity) {
      if (entity == nullptr) {
         return;
      }

      this->bookmark(entity->id(), entity->handle());
   }

   void remove_bookmark(const std::string& bookmark_id) {
      auto b = this->bookmarks_.find(bookmark_id);

      if (b != this->bookmarks_.end()) {
         this->bookmarks_.erase(b);
      }
   }

private:
   std::string id_;
   bool is_initialized_;

   Game* game_;

   FontAtlas fonts_;
   TextureAtlas textures_;

   ObjectPool<Entity> entities_;
   ComponentManager components_;
   std::vector<System*> systems_; // use this to update systems in priority order
   SystemHash system_hash_; // use this to add/remove systems

   Handle root_;
   std::map<std::string, Handle> bookmarks_;

   SceneNode* scene_graph_;

   // whenever Scene receives a message, we want to propagate it to messageables inside
   virtual void post_receive_message(MessagePtr message) {
      // broadcast a message to all systems in the scene
      for (std::vector<System*>::const_iterator it = this->systems_.begin(); it != this->systems_.end(); ++it) {
         (*it)->receive_message(message);
      }

      // broadcast a message to all entities in the scene
      std::vector<Handle> entities = this->entities();
      for (std::vector<Handle>::const_iterator it = entities.begin(); it != entities.end(); ++it) {
         this->get_entity(*it)->receive_message(message);
      }
   }

   virtual void send_message_helper(MessagePtr message) {
      this->receive_message(message);
   }
};

#endif
