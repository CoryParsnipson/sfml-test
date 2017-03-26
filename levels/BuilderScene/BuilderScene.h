#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include "Scene.h"
#include "Settings.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Channel;
class Texture;
class Map;

// ----------------------------------------------------------------------------
// BuilderScene
// ----------------------------------------------------------------------------
class BuilderScene
: public Scene
{
public:
   BuilderScene();
   virtual ~BuilderScene();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // update interface
   virtual void update(Game& game);

   // input event processing
   virtual void process(Game& game, CloseInputEvent& e);
   virtual void process(Game& game, ResizeInputEvent& e);

protected:
   // scene fixtures
   Camera* hud_camera_;
   Camera* map_camera_;
   Entity* backdrop_;

   Channel* map_file_;
   Map* map_;

   // entity pointers (no ownership)
   Entity* center_dot_;
   Entity* fps_display_;
   Entity* tile_cursor_;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;

   // helper methods
   void update_fps(Settings& settings);
};

#endif
