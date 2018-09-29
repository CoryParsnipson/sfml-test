#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include <functional>

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
   bool grid_visible_;
   bool visual_debug_enable_;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;

   void load_fonts();
   void load_textures();

   // helper methods
   Handle create_panel(std::string entity_id, sf::FloatRect bounds, bool create_decoration = false, std::string label = "");
   Handle create_button(std::string entity_id, sf::FloatRect bounds, std::string button_text = "Button", std::function<void()> action = nullptr);

   void create_hud(Game& game);

   void setup_keybindings(Game& game);

   void create_mouse_entity(Game& game);
   void create_backdrop(GraphicalSystem* gs);
   void create_fps_display(GraphicalSystem* gs);
   void create_tile_palette(GraphicalSystem* gs, std::string& tileset_data);

   Handle create_notification();
};

#endif
