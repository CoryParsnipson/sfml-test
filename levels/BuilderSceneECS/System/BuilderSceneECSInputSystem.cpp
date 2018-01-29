#include "BuilderSceneECSInputSystem.h"

#include "Game.h"
#include "Scene.h"

#include "GridVisibilityToggleIntent.h"
#include "VisualDebugIntent.h"
#include "ResetCameraIntent.h"
#include "RemoveTilesIntent.h"
#include "MoveUpIntent.h"
#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"
#include "MoveDownIntent.h"

#include "TileMap.h"
#include "Collision.h"

#include "SetGridVisibilityMessage.h"
#include "SetVisualDebugMessage.h"

BuilderSceneECSInputSystem::BuilderSceneECSInputSystem(const std::string& id /* = "BuilderSceneECSInputSystem" */)
: System(id)
, grid_visible_(false)
, grid_visible_button_down_(false)
, visual_debug_enable_(true)
, visual_debug_enable_down_(false)
, reset_camera_down_(false)
, remove_tiles_down_(false)
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

   bool reset_camera = game.get_player(1).bindings().get<ResetCameraIntent>()->element()->is_pressed();
   if (!this->reset_camera_down_ && reset_camera) {
      this->map_camera->reset_zoom();
      this->map_camera->reset_pan();

      this->reset_camera_down_ = reset_camera;
   }

   if (this->reset_camera_down_ && !reset_camera) {
      this->reset_camera_down_ = reset_camera;
   }

   if (game.get_player(1).bindings().get<MoveUpIntent>()->element()->is_pressed()) {
      this->map_camera->move(sf::Vector2f(0, -10));
   }

   if (game.get_player(1).bindings().get<MoveLeftIntent>()->element()->is_pressed()) {
      this->map_camera->move(sf::Vector2f(-10, 0));
   }

   if (game.get_player(1).bindings().get<MoveRightIntent>()->element()->is_pressed()) {
      this->map_camera->move(sf::Vector2f(10, 0));
   }

   if (game.get_player(1).bindings().get<MoveDownIntent>()->element()->is_pressed()) {
      this->map_camera->move(sf::Vector2f(0, 10));
   }

   bool remove_tiles = game.get_player(1).bindings().get<RemoveTilesIntent>()->element()->is_pressed();
   if (remove_tiles && !this->remove_tiles_down_) {
      Entity* map_entity = this->scene().get_entity(this->map_entity);
      Entity* tile_selection_entity = this->scene().get_entity(this->tile_selection);

      if (map_entity && tile_selection_entity) {
         TileMap* map = map_entity->get<TileMap>();
         Collision* collision = tile_selection_entity->get<Collision>();

         if (map && collision) {
            map->remove(collision->volume());
         }
      }

      this->remove_tiles_down_ = remove_tiles;
   }

   if (!remove_tiles && this->remove_tiles_down_) {
      this->remove_tiles_down_ = remove_tiles;
   }
}
