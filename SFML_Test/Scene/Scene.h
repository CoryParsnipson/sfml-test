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

#include "CameraSceneGraphNode.h"

class Scene
: public Draw
, public Update
{
public:
   using SceneGraph = std::map<int, SceneGraphNode*>;

   Scene(std::string id)
   : id_(id)
   , camera_(new Camera("Camera", sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height())))
   {
      Service::get_logger().msg(this->id(), Logger::INFO, "Creating new Scene.");
      this->scene_graph_[0] = new CameraSceneGraphNode(*this->camera_);
   }

   virtual ~Scene() {
      Service::get_logger().msg(this->id(), Logger::INFO, "Destroying Scene.");

      delete this->camera_;
      this->camera_ = nullptr;

      SceneGraph::iterator it;
      for (it = this->scene_graph_.begin(); it != this->scene_graph_.end(); ++it) {
         delete it->second;
         this->scene_graph_.erase(it);
      }
      this->scene_graph_.clear();
   }

   virtual std::string id() { return this->id_; }

   virtual void enter(Game& game) {}
   virtual void exit(Game& game) {}

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {
      SceneGraph::iterator it;
      for (it = this->scene_graph_.begin(); it != this->scene_graph_.end(); ++it) {
         it->second->draw(surface, render_states);
      }
   }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) {
      SceneGraph::iterator it;
      for (it = this->scene_graph_.begin(); it != this->scene_graph_.end(); ++it) {
         SceneGraphNode::prefix_iterator node_it;
         for (node_it = it->second->begin(); node_it != it->second->end(); ++node_it) {
            node_it->update(game, this);
         }
      }
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
   SceneGraph scene_graph_;
};

#endif
