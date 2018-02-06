#include "StartMenuInputSystem.h"
#include "BuilderScene.h"

#include "Game.h"

StartMenuInputSystem::StartMenuInputSystem(const std::string& id /* = "StartMenuInputSystem" */)
: System(id)
, change_scene_down_(false)
{
}

StartMenuInputSystem::~StartMenuInputSystem() {
}

void StartMenuInputSystem::pre_update(Game& game) {
   bool is_change_scene_down = game.input_manager().get_device(1)->get("Space")->is_pressed() || game.input_manager().get_device(1)->get("Return")->is_pressed();

   if (is_change_scene_down && !this->change_scene_down_) {
      this->scene().switch_scene(new BuilderScene());
      this->change_scene_down_ = is_change_scene_down;
   }

   if (!is_change_scene_down && this->change_scene_down_) {
      this->change_scene_down_ = is_change_scene_down;
   }
}
