#ifndef START_MENU_ECS_INPUT_SYSTEM_H
#define START_MENU_ECS_INPUT_SYSTEM_H

#include "System.h"

// ----------------------------------------------------------------------------
// StartMenuECSInputSystem
//
// This handles actions that should occur due to keyboard input.
// ----------------------------------------------------------------------------
class StartMenuECSInputSystem : public System {
public:
   StartMenuECSInputSystem(const std::string& id = "StartMenuECSInputSystem");
   virtual ~StartMenuECSInputSystem();

private:
   virtual void on_init(Game& game) {}
   virtual void pre_update(Game& game);
   virtual void on_update(Game& game, Entity& e) {}

   bool change_scene_down_;
};

#endif
