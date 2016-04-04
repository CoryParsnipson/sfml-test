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
#include "Update.h"
#include "Viewport.h"
#include "Entity.h"

class Scene
: public Draw
, public Update
{
public:
   typedef std::vector<Entity*> EntityList;

   Scene(std::string id)
   : id_(id)
   , viewport_(new Viewport(sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height())))
   {
      Service::get_logger().msg(this->id(), Logger::INFO, "Creating new Scene.");
   }

   virtual ~Scene() {
      Service::get_logger().msg(this->id(), Logger::INFO, "Destroying Scene.");

      delete this->viewport_;

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
   virtual void draw(RenderTarget& surface) {
      this->viewport_->draw(surface);
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
      sf::Vector2f new_size(c.width, c.height);
      sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

      // update viewport
      this->viewport_->resize(new_size);
      this->viewport_->recenter(new_center);
   }
   virtual void process(Game& game, MouseMoveCommand& c) {}
   virtual void process(Game& game, MouseButtonCommand& c) {}
   virtual void process(Game& game, MouseWheelCommand& c) {}

protected:
   std::string id_;

   Viewport* viewport_;
   EntityList entities_;
};

#endif
