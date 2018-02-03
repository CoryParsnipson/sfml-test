#include "BuilderSceneECSInputSystem.h"

#include "Game.h"
#include "Scene.h"

#include "GridVisibilityToggleIntent.h"
#include "VisualDebugIntent.h"
#include "ResetViewIntent.h"
#include "RemoveTilesIntent.h"
#include "MoveUpIntent.h"
#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"
#include "MoveDownIntent.h"
#include "SerializeMapIntent.h"

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
, save_map_down_(false)
, clock()
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
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

   bool reset_camera = game.get_player(1).bindings().get<ResetViewIntent>()->element()->is_pressed();
   if (!this->reset_camera_down_ && reset_camera) {
      Entity* map_entity = this->scene().get_entity(this->map_entity);
      map_entity->get<Space>()->position(0, 0);
      map_entity->get<Space>()->scale(sf::Vector2f(1.f, 1.f));

      this->reset_camera_down_ = reset_camera;
   }

   if (this->reset_camera_down_ && !reset_camera) {
      this->reset_camera_down_ = reset_camera;
   }

   if (game.get_player(1).bindings().get<MoveUpIntent>()->element()->is_pressed()) {
      Entity* map_entity = this->scene().get_entity(this->map_entity);
      map_entity->get<Space>()->move(sf::Vector2f(0, -10));
   }

   if (game.get_player(1).bindings().get<MoveLeftIntent>()->element()->is_pressed()) {
      Entity* map_entity = this->scene().get_entity(this->map_entity);
      map_entity->get<Space>()->move(sf::Vector2f(-10, 0));
   }

   if (game.get_player(1).bindings().get<MoveRightIntent>()->element()->is_pressed()) {
      Entity* map_entity = this->scene().get_entity(this->map_entity);
      map_entity->get<Space>()->move(sf::Vector2f(10, 0));
   }

   if (game.get_player(1).bindings().get<MoveDownIntent>()->element()->is_pressed()) {
      Entity* map_entity = this->scene().get_entity(this->map_entity);
      map_entity->get<Space>()->move(sf::Vector2f(0, 10));
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

   bool save_map = game.get_player(1).bindings().get<SerializeMapIntent>()->element()->is_pressed();
   if (save_map && !this->save_map_down_) {
      Entity* map_root = this->scene().get_entity(this->map_entity);
      TileMap* tilemap = map_root->get<TileMap>();

      this->file_channel->remove();
      this->file_channel->seek(0);
      this->file_channel->send(tilemap->serialize(*this->serializer));

      Game::logger().msg("BuilderSceneECSInputSystem", Logger::INFO, "Saving map to file '" + this->file_channel->filename());
      this->save_map_down_ = save_map;
   }

   if (!save_map && this->save_map_down_) {
      this->save_map_down_ = save_map;
   }

   // update fps read
   if (!this->frame_count) {
      this->last_frame_time = (((float)this->frame_measurement_interval / this->clock.getElapsedTime().asSeconds()) * game.settings.framerate_smoothing())
                              + (this->last_frame_time * (1.0 - game.settings.framerate_smoothing()));
      this->clock.restart();

      // update fps entity
      Entity* fps_entity = this->scene().get_entity(this->fps_entity);
      fps_entity->get<Text>()->string("FPS: " + std::to_string(this->last_frame_time));
   }
   this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
}
