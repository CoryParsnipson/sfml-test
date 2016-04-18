#ifndef SCENE_H
#define SCENE_H

#include "dependencies.h"

#include "CloseCommand.h"
#include "KeyPressCommand.h"
#include "WindowResizeCommand.h"
#include "MouseMoveCommand.h"
#include "MouseButtonCommand.h"
#include "MouseWheelCommand.h"

#include "Game.h"
#include "Draw.h"
#include "Camera.h"
#include "Update.h"
#include "Entity.h"

#include "SceneGraphNode.h"

class Scene
: public Draw
, public Update
{
public:
   using SceneGraph = std::map<int, SceneGraphNode*>;

   Scene(std::string id)
   : id_(id)
   , camera_(new Camera("Camera", sf::Vector2f(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT)))
   {
      Service::get_logger().msg(this->id(), Logger::INFO, "Creating new Scene.");
      this->scene_graph_[0] = new SceneGraphNode(this->camera_);
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
            // TODO: figure something out for this
            //(*node_it)->update(game, this);

            Entity* e = dynamic_cast<Entity*>(*node_it);
            if (e) {
               e->update(game, this);
            }
         }
      }
   }

   // command interface default implementations
   virtual void process(Game& game, CloseCommand& c) {
      game.unload_scene();
   }

   virtual void process(Game& game, KeyPressCommand& c) {}

   virtual void process(Game& game, WindowResizeCommand& c) {
      // update main camera
      this->camera_->set_size(sf::Vector2f(c.width, c.height));
      this->camera_->set_center(sf::Vector2f(c.width / 2.f, c.height / 2.f));
   }

   virtual void process(Game& game, MouseMoveCommand& c) {}
   virtual void process(Game& game, MouseButtonCommand& c) {}
   virtual void process(Game& game, MouseWheelCommand& c) {}

protected:
   std::string id_;
   Camera* camera_;
   SceneGraph scene_graph_;
};

#endif
