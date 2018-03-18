#ifndef BUILDER_SCENE_INPUT_SYSTEM_H
#define BUILDER_SCENE_INPUT_SYSTEM_H

#include "System.h"
#include "Camera.h"
#include "Serializer.h"
#include "FileChannel.h"

// ----------------------------------------------------------------------------
// BuilderSceneInputSystem
//
// This handles actions that should occur due to keyboard input.
// ----------------------------------------------------------------------------
class BuilderSceneInputSystem : public System {
public:
   BuilderSceneInputSystem(const std::string& id = "BuilderSceneInputSystem");
   virtual ~BuilderSceneInputSystem();

   Handle grid_entity;
   Handle map_entity;
   Handle tile_selection;
   Handle tile_selection_maproot;
   Handle fps_entity;

   Serializer* serializer;
   FileChannel* file_channel;

private:
   virtual void on_init(Game& game) {}
   virtual void pre_update(Game& game);
   virtual void on_update(Game& game, Entity& e) {}

   bool grid_visible_;
   bool visual_debug_enable_;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;
};

#endif
