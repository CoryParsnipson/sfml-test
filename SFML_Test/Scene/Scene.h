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
   using EntityList = std::vector<Entity*>;
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

      SceneGraph::iterator sg_it;
      for (sg_it = this->scene_graph_.begin(); sg_it != this->scene_graph_.end(); ++sg_it) {
         delete sg_it->second;
      }

      EntityList::iterator it;
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         delete *it;
         *it = nullptr;
      }
      this->entities_.clear();
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
      EntityList::const_iterator it;
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         (*it)->update(game, this);
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
   EntityList entities_;

   SceneGraph scene_graph_;
};

#endif
