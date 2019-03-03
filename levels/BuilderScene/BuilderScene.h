#ifndef BUILDER_SCENE_H
#define BUILDER_SCENE_H

#include <memory>
#include <functional>

#include "Scene.h"
#include "Settings.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Channel;
class Texture;
class Map;
class Tileset;

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

   int next_map_idx_;
   Handle active_map_layer_;

   std::string scene_data_filename;

   std::vector<std::shared_ptr<Tileset>> tilesets_;

   void load_fonts();
   void load_textures();

   // helper methods
   Handle create_panel(std::string entity_id, sf::FloatRect bounds, bool create_decoration = false, std::string label = "");
   Handle create_button(std::string entity_id, sf::FloatRect bounds, std::string button_text = "Button", Color button_bg_color = Color(113, 94, 122));

   void create_hud(Game& game);
   void load_file_dialog_box();
   void save_file_dialog_box();

   void setup_keybindings(Game& game);
   void load_from_file(std::string filename);
   void save_to_file(std::string filename);

   void unload_scene_data();

   void create_mouse_entity(Game& game);
   void create_backdrop(GraphicalSystem* gs);
   void create_fps_display(GraphicalSystem* gs);

   void hide_entity(std::string entity_id);
   void show_entity(std::string entity_id);

   Handle add_map_layer();
   void remove_map_layer(int map_idx);
   void clear_map_layers();

   void set_active_layer(Handle map_layer);
   Handle get_active_layer();

   void create_tile_palette();
   void populate_tile_palette(Tileset& tileset);
   void clear_tile_palette();

   void create_layers_panel();
   void populate_layers_panel();
   void clear_layers_panel();

   void mouse_script_add_zoom_behavior(Game& game, Handle mouse_entity);
   void mouse_script_add_pan_behavior(Game& game, Handle mouse_entity);
   void mouse_script_add_select_behavior(Game& game, Handle mouse_entity);
   void mouse_script_add_move_behavior(Game& game, Handle mouse_entity, bool is_clicked_initial = false);

   Handle create_notification(float width, float height);
   Handle create_dialog_box(std::string display_text, sf::Vector2f size);
   Handle create_textbox(std::string entity_id, float width, int charsize = 12, unsigned int max_len = 30);
   Handle create_dropdown(std::string entity_id, std::string dropdown_button_text, sf::FloatRect button_bounds, sf::FloatRect dropdown_bounds);
};

#endif
