#ifndef SCENE_H
#define SCENE_H

#include "dependencies.h"

#include "CloseCommand.h"
#include "KeyPressCommand.h"
#include "WindowResizeCommand.h"
#include "MouseMoveCommand.h"
#include "MouseButtonCommand.h"
#include "MouseWheelCommand.h"

#include "Draw.h"
#include "Update.h"
#include "Viewport.h"
#include "Entity.h"

class Game;

class Scene
: public Draw
, public Update
{
public:
   typedef std::vector<Entity*> EntityList;

   Scene(std::string id)
   : id_(id)
   , viewport_(nullptr)
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
   virtual void draw(Graphics& graphics) {
      this->viewport_->draw(graphics);
   }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) {
      EntityList::const_iterator it;
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         (*it)->update(game, this);
      }
   }

   // command interface (TODO: get rid of this?)
   virtual void process(Game& game, CloseCommand& c) = 0;
   virtual void process(Game& game, KeyPressCommand& c) = 0;
   virtual void process(Game& game, WindowResizeCommand& c) = 0;
   virtual void process(Game& game, MouseMoveCommand& c) = 0;
   virtual void process(Game& game, MouseButtonCommand& c) = 0;
   virtual void process(Game& game, MouseWheelCommand& c) = 0;

protected:
   std::string id_;

   Viewport* viewport_;
   EntityList entities_;
};

#endif
