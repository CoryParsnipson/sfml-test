#ifndef SCENE_H
#define SCENE_H

#include "dependencies.h"

#include "CloseCommand.h"
#include "KeyPressCommand.h"
#include "WindowResizeCommand.h"
#include "MouseMoveCommand.h"
#include "MouseButtonCommand.h"
#include "MouseWheelCommand.h"

#include "Viewport.h"

// forward declarations
class Game;

class Scene {
public:
   Scene() {}
   virtual ~Scene() {
      std::map<std::string, Viewport*>::const_iterator it;
      for (it = this->viewports_.begin(); it != this->viewports_.end(); ++it) {
         delete it->second;
      }
      this->viewports_.clear();
   }

   virtual void enter(Game& game) {}
   virtual void exit(Game& game) {}

   virtual void update(Game& game) = 0;

   // command interface
   virtual void process(Game& game, CloseCommand& c) = 0;
   virtual void process(Game& game, KeyPressCommand& c) = 0;
   virtual void process(Game& game, WindowResizeCommand& c) = 0;
   virtual void process(Game& game, MouseMoveCommand& c) = 0;
   virtual void process(Game& game, MouseButtonCommand& c) = 0;
   virtual void process(Game& game, MouseWheelCommand& c) = 0;

protected:
   bool is_running_; // false to exit scene
   std::map<std::string, Viewport*> viewports_; // TODO: may need to replace this with some newer class
};

#endif
