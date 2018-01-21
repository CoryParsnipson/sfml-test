#ifndef BUILDER_SCENE_ECS_INPUT_SYSTEM_H
#define BUILDER_SCENE_ECS_INPUT_SYSTEM_H

#include "System.h"
#include "Camera.h"

// ----------------------------------------------------------------------------
// BuilderSceneECSInputSystem
//
// This handles actions that should occur due to keyboard input.
// ----------------------------------------------------------------------------
class BuilderSceneECSInputSystem : public System {
public:
   BuilderSceneECSInputSystem(const std::string& id = "BuilderSceneECSInputSystem");
   virtual ~BuilderSceneECSInputSystem();

   Handle grid_entity;
   CameraPtr map_camera;

private:
   virtual void on_init(Game& game) {}
   virtual void pre_update(Game& game);
   virtual void on_update(Game& game, Entity& e) {}

   bool grid_visible_;
   bool grid_visible_button_down_;

   bool visual_debug_enable_;
   bool visual_debug_enable_down_;

   bool reset_camera_down_;
};

#endif