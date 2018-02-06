#ifndef START_MENU_INPUT_SYSTEM_H
#define START_MENU_INPUT_SYSTEM_H

#include "System.h"

// ----------------------------------------------------------------------------
// StartMenuInputSystem
//
// This handles actions that should occur due to keyboard input.
// ----------------------------------------------------------------------------
class StartMenuInputSystem : public System {
public:
   StartMenuInputSystem(const std::string& id = "StartMenuInputSystem");
   virtual ~StartMenuInputSystem();

private:
   virtual void on_init(Game& game) {}
   virtual void pre_update(Game& game);
   virtual void on_update(Game& game, Entity& e) {}

   bool change_scene_down_;
};

#endif
