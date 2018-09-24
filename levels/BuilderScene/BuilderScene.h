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

   virtual void init(Game& game);
   virtual void enter(Game& game);
   virtual void exit(Game& game);

   // input event processing
   virtual void process(Game& game, CloseInputEvent& e);
   virtual void process(Game& game, MouseEnteredInputEvent& e);
   virtual void process(Game& game, MouseLeftInputEvent& e);

private:
   Handle mouse_cursor_;

   bool grid_visible_;
   bool visual_debug_enable_;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;

   void load_fonts();
   void load_textures();

   // helper methods
   Handle create_panel(std::string entity_id, sf::FloatRect bounds, std::string label = "");

   void create_backdrop(GraphicalSystem* gs);
   void create_fps_display(GraphicalSystem* gs);
   void create_tile_palette(GraphicalSystem* gs);
};

#endif
