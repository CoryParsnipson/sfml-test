#include "BuilderSceneECSInputSystem.h"

#include "Game.h"

#include "GridVisibilityToggleIntent.h"
#include "VisualDebugIntent.h"

#include "SetGridVisibilityMessage.h"
#include "SetVisualDebugMessage.h"

BuilderSceneECSInputSystem::BuilderSceneECSInputSystem(const std::string& id /* = "BuilderSceneECSInputSystem" */)
: System(id)
, visual_debug_enable_(true)
{
}

BuilderSceneECSInputSystem::~BuilderSceneECSInputSystem() {
}

void BuilderSceneECSInputSystem::pre_update(Game& game) {
   bool new_grid_visibility = game.get_player(1).bindings().get<GridVisibilityToggleIntent>()->element()->is_pressed();
   bool new_visual_debug_enable = game.get_player(1).bindings().get<VisualDebugIntent>()->element()->is_pressed();

   if (new_grid_visibility && this->grid_visible_button_down_ != new_grid_visibility) {
      this->send_message_async<SetGridVisibilityMessage>(this->grid_entity, this->grid_visible_);
      this->grid_visible_ = !this->grid_visible_;
      this->grid_visible_button_down_ = new_grid_visibility;
   }

   if (!new_grid_visibility && this->grid_visible_button_down_ != new_grid_visibility) {
      this->grid_visible_button_down_ = new_grid_visibility;
   }

   if (new_visual_debug_enable && this->visual_debug_enable_down_ != new_visual_debug_enable) {
      this->send_message_async<SetVisualDebugMessage>(this->visual_debug_enable_);
      this->visual_debug_enable_ = !this->visual_debug_enable_;
      this->visual_debug_enable_down_ = new_visual_debug_enable;
   }

   if (!new_visual_debug_enable && this->visual_debug_enable_down_ != new_visual_debug_enable) {
      this->visual_debug_enable_down_ = new_visual_debug_enable;
   }
}
