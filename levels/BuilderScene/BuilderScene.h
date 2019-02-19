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
   virtual void process(Game& game, GainedFocusInputEvent& e);

private:
   bool grid_visible_;
   bool visual_debug_enable_;

   sf::Clock clock;
   int last_frame_time;
   int frame_measurement_interval;
   int frame_count;

   std::string scene_data_filename;

   void load_fonts();
   void load_textures();

   // helper methods
   Handle create_panel(std::string entity_id, sf::FloatRect bounds, bool create_decoration = false, std::string label = "");
   Handle create_button(std::string entity_id, sf::FloatRect bounds, std::string button_text = "Button", std::function<void()> action = nullptr);

   void create_hud(Game& game);
   void file_dialog_box();

   void setup_keybindings(Game& game);
   void load_from_file(std::string filename);

   void create_mouse_entity(Game& game);
   void create_backdrop(GraphicalSystem* gs);
   void create_fps_display(GraphicalSystem* gs);
   void create_tile_palette(GraphicalSystem* gs);

   void mouse_script_add_zoom_behavior(Game& game, Handle mouse_entity);
   void mouse_script_add_pan_behavior(Game& game, Handle mouse_entity);
   void mouse_script_add_select_behavior(Game& game, Handle mouse_entity);
   void mouse_script_add_move_behavior(Game& game, Handle mouse_entity, bool is_clicked_initial = false);

   Handle create_notification(float width, float height);
   Handle create_textbox(std::string entity_id, float width, int charsize = 12, unsigned int max_len = 30);
   Handle create_dropdown(std::string entity_id, std::string dropdown_button_text, sf::FloatRect button_bounds, sf::FloatRect dropdown_bounds);
};

#endif
