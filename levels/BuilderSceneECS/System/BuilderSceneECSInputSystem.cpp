#include "BuilderSceneECSInputSystem.h"
#include "GridVisibilityToggleIntent.h"
#include "SetGridVisibilityMessage.h"
#include "Game.h"

BuilderSceneECSInputSystem::BuilderSceneECSInputSystem(const std::string& id /* = "BuilderSceneECSInputSystem" */)
: System(id)
{
}

BuilderSceneECSInputSystem::~BuilderSceneECSInputSystem() {
}

void BuilderSceneECSInputSystem::pre_update(Game& game) {
   bool new_grid_visibility = game.get_player(1).bindings().get<GridVisibilityToggleIntent>()->element()->is_pressed();

   if (new_grid_visibility && this->grid_visible_button_down_ != new_grid_visibility) {
      this->send_message_async<SetGridVisibilityMessage>(this->grid_entity, this->grid_visible_);

      // update grid visibility toggle
      this->grid_visible_ = !this->grid_visible_;

      // update grid visibility button state
      this->grid_visible_button_down_ = new_grid_visibility;
   }

   if (!new_grid_visibility && this->grid_visible_button_down_ != new_grid_visibility) {
      // update grid visibility button state
      this->grid_visible_button_down_ = new_grid_visibility;
   }
}
