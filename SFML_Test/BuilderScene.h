#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include "Scene.h"

class Texture;

class Map;
class MapBuilder;
class Serializer;

class BuilderScene
: public Scene
{
public:
   BuilderScene();
   virtual ~BuilderScene();

   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

   // input event processing
   virtual void process(Game& game, CloseInputEvent& e);
   virtual void process(Game& game, ResizeInputEvent& e);
   virtual void process(Game& game, KeyPressInputEvent& e);

protected:
   // scene fixtures
   Map* map_;
   Camera* hud_camera_;
   Camera* map_camera_;
   Entity* backdrop_;
   std::string map_filename_;
   Serializer* serializer_;

   // entity pointers (no ownership)
   Entity* center_dot_;
   Entity* fps_display_;
   Entity* tile_cursor_;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;
   bool show_debug_info_;

   // helper methods
   void update_fps();
   void toggle_debug_info();

   void set_tiles(Texture& tile_texture);
   void remove_tiles();
};

#endif
