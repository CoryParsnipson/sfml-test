#include <cmath>
#include <cstdio>
#include <limits>

#include <SFML/Graphics.hpp>

#include "BuilderScene.h"

#include "Game.h"
#include "Entity.h"

#include "Color.h"

#include "Text.h"
#include "Rectangle.h"
#include "VertexList.h"
#include "Callback.h"
#include "Clickable.h"
#include "Collision.h"
#include "PlayerProfile.h"
#include "TileMap.h"
#include "Grid.h"

#include "FileChannel.h"
#include "JSONSerializer.h"

#include "MouseXIntent.h"
#include "MouseYIntent.h"
#include "MouseWheelIntent.h"
#include "LeftClickIntent.h"
#include "RightClickIntent.h"
#include "VisualDebugIntent.h"
#include "GridVisibilityToggleIntent.h"
#include "ResetViewIntent.h"
#include "RemoveTilesIntent.h"
#include "MoveUpIntent.h"
#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"
#include "MoveDownIntent.h"
#include "SerializeMapIntent.h"
#include "ModalIntent.h"
#include "UseMoveToolIntent.h"

#include "GraphicalSystem.h"
#include "VisualDebugSystem.h"
#include "GridSystem.h"

#include "SetGridVisibilityMessage.h"
#include "SetVisualDebugMessage.h"
#include "EntityIdChangedMessage.h"

#define KB_CAPTURE(keycode, key, shifted_key) \
   if (keyboard->get(keycode)->was_pressed()) { \
      new_char = keyboard->get("LShift")->is_pressed() || keyboard->get("RShift")->is_pressed() ? shifted_key : key; \
   }

BuilderScene::BuilderScene()
: Scene("BuilderScene")
, grid_visible_(false)
, visual_debug_enable_(true)
, clock()
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
, scene_data_filename()
{
}

BuilderScene::~BuilderScene() {
}

void BuilderScene::init(Game& game) {
   this->load_fonts();
   this->load_textures();

   // make map_root, grid_root, and hud_root (ordering is important here for layering reasons)
   Entity* map_root = this->create_entity("MapRootEntity");
   map_root->add<TileMap>();
   map_root->space()->position(0, 30); // move so origin is not obscured by menubar

   Entity* grid_root = this->create_entity("GridRootEntity");
   grid_root->space()->position(0, 30); // move so origin is not obscured by menubar

   Entity* grid_entity = this->create_entity("GridEntity");
   grid_entity->add<Grid>("Grid Component");
   this->add_to_scene_node(grid_root, grid_entity);

   this->create_entity("HudRootEntity");

   GraphicalSystem* gs = this->get_system<GraphicalSystem>("GraphicalSystem");

   VisualDebugSystem* vds = new VisualDebugSystem("VisualDebugSystem", game.window(), gs->camera());
   vds->disable(); // start with this off
   this->add_system(vds);

   // add grid system
   this->add_system(new GridSystem("GridSystem", gs->camera()));

   this->create_hud(game); // make all UI entities

   this->create_mouse_entity(game);
   this->create_backdrop(gs);

   this->setup_keybindings(game);
}

void BuilderScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering builder state.");
}

void BuilderScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting builder state.");
}

void BuilderScene::process(Game& game, CloseInputEvent& e) {
   game.unload_scene();
}

void BuilderScene::process(Game& game, MouseEnteredInputEvent& e) {
   Entity* cursor = this->get_entity("MouseCursorEntity");
   if (cursor) {
      cursor->space()->visible(true);
   }
}

void BuilderScene::process(Game& game, MouseLeftInputEvent& e) {
   Entity* cursor = this->get_entity("MouseCursorEntity");
   if (cursor) {
      cursor->space()->visible(false);
   }
}

void BuilderScene::process(Game& game, GainedFocusInputEvent& e) {
   Entity* cursor = this->get_entity("MouseCursorEntity");
   if (cursor) {
      cursor->space()->visible(true);
   }
}

void BuilderScene::load_fonts() {
   // load fonts
   this->fonts().load("retro", "retro.ttf");
}

void BuilderScene::load_textures() {
   this->textures().load("ui_close_button", "ui_close_button.png");
   this->textures().load("cursors", "mouse_cursor.png");
}

Handle BuilderScene::create_panel(std::string entity_id, sf::FloatRect bounds, bool create_decoration /* = false */, std::string label /* = "" */) {
   Entity* panel = this->create_entity(entity_id);

   // UI colors
   Color panel_bg_color(113, 94, 122, 255);
   Color panel_bg_highlight(211, 206, 218, 230);
   Color panel_bg_transparent(sf::Color::Transparent);

   // calculate label offsets
   int label_offset_top = 0;
   int label_offset_left = bounds.width / 8;
   
   int label_padding_top = 2;
   int label_padding_bottom = 0;
   int label_padding_left = 3;
   int label_padding_right = 3;

   int padding_top = (label != "" ? 10 : 5);
   int padding_bottom = 5;
   int padding_left = 5;
   int padding_right = 5;

   // create panel background
   panel->add<Rectangle>(entity_id + "Rectangle", 0, 0, bounds.width, bounds.height);
   panel->get<Rectangle>()->color(panel_bg_color);
   panel->space()->position(bounds.left, bounds.top);

   // create label and decoration if necessary
   if (create_decoration) {
      Entity* panel_outline = this->create_entity(entity_id + "Decoration");
      panel_outline->add<Rectangle>(
         entity_id + "DecorationRectangle",
         0,
         0,
         bounds.width - padding_left - padding_right,
         bounds.height - padding_top - padding_bottom
      );
      panel_outline->get<Rectangle>()->color(panel_bg_transparent);
      panel_outline->get<Rectangle>()->outline_color(panel_bg_highlight);
      panel_outline->get<Rectangle>()->outline_thickness(2.0);

      panel_outline->space()->position(padding_left, padding_top);

      this->add_to_scene_node(panel, panel_outline);
   }

   if (label != "") {
      Entity* panel_title_back = this->create_entity(entity_id + "LabelBackground");
      panel_title_back->add<Rectangle>(entity_id + "LabelBackgroundRectangle", 0, 0, 0, 0);
      panel_title_back->get<Rectangle>()->color(panel_bg_color);
      panel_title_back->space()->position(label_offset_left, label_offset_top);

      this->add_to_scene_node(panel, panel_title_back);

      Entity* panel_title = this->create_entity(entity_id + "Label");
      panel_title->add<Text>(entity_id + "LabelText", label, this->fonts().get("retro"), 12);
      panel_title->space()->position(label_offset_left + label_padding_left, label_offset_top + label_padding_top);

      this->add_to_scene_node(panel, panel_title);

      panel_title_back->get<Rectangle>()->size(
         panel_title->get<Text>()->local_bounds().width + label_padding_left + label_padding_right,
         panel_title->get<Text>()->local_bounds().height + label_padding_top + label_padding_bottom
      );
   }

   return panel->handle();
}

Handle BuilderScene::create_button(std::string entity_id, sf::FloatRect bounds, std::string button_text /* = "Button" */, std::function<void()> action /* = nullptr */) {
   Color button_bg_color(113, 94, 122, 255);
   Color button_bg_highlight(141, 116, 153, 255);
   Color button_bg_down(103, 84, 112, 255);

   Entity* button_text_entity = this->create_entity(entity_id + "ButtonText");

   button_text_entity->add<Text>(entity_id + "ButtonTextText", button_text, this->fonts().get("retro"), 12);

   if (bounds.width == 0) {
      // assume this means fit to button text width
      bounds.width = button_text_entity->get<Text>()->local_bounds().width + 20;
   }

   if (bounds.height == 0) {
      // assume this means fit to button text height
      bounds.height = button_text_entity->get<Text>()->local_bounds().height + 20;
   }

   Entity* button = this->get_entity(this->create_panel(entity_id, bounds, true));

   button->add<PlayerProfile>(entity_id + "PlayerProfile", 1);
   button->add<Collision>(entity_id + "Collision", button->get<Rectangle>()->local_bounds());
   button->add<Clickable>(entity_id + "Clickable");

   button->add<Callback>(entity_id + "Callback", false);
   if (action) {
      button->get<Callback>()->left_release(action);
   }
   
   // put common callback functions on a different entity on top of the button entity
   // so that users who override callbacks on the button won't overwrite these
   // NOTE: if the size of the button changes, you need to remember to change the
   // size of this entity's collision volume too...
   Entity* button_script_overlay = this->create_entity(entity_id + "ScriptOverlay");

   button_script_overlay->add<PlayerProfile>(entity_id + "PlayerProfile", 1);
   button_script_overlay->add<Collision>(entity_id + "Collision", button->get<Rectangle>()->local_bounds());
   button_script_overlay->add<Clickable>(entity_id + "Clickable");
   button_script_overlay->add<Callback>(entity_id + "Callback", true);

   button_script_overlay->get<Callback>()->mouse_in([button, button_bg_highlight] () {
      button->get<Rectangle>()->color(button_bg_highlight);
   });

   button_script_overlay->get<Callback>()->mouse_out([button, button_bg_color] () {
      button->get<Rectangle>()->color(button_bg_color);
   });

   button_script_overlay->get<Callback>()->left_click([button, button_bg_down] () {
      button->get<Rectangle>()->color(button_bg_down);
   });

   button_script_overlay->get<Callback>()->left_release([button, button_bg_color, button_bg_highlight, this] () {
      sf::Vector2f mouse_pos(
         this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position(),
         this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position()
      );
      sf::FloatRect button_volume = this->global_transform(*button).transformRect(button->get<Collision>()->volume());

      button->get<Rectangle>()->color(button_volume.contains(mouse_pos) ? button_bg_highlight : button_bg_color);
   });

   this->add_to_scene_node(button, button_script_overlay);

   // center button text
   button_text_entity->get<Text>()->origin(
      button_text_entity->get<Text>()->local_bounds().left + button_text_entity->get<Text>()->local_bounds().width / 2.f,
      button_text_entity->get<Text>()->local_bounds().top + button_text_entity->get<Text>()->local_bounds().height/ 2.f
   );
   button_text_entity->get<Text>()->offset(button->get<Rectangle>()->size() / 2.f);

   this->add_to_scene_node(button, button_text_entity);

   return button->handle();
}

void BuilderScene::create_hud(Game& game) {
   Entity* hud_root = this->get_entity("HudRootEntity");
   Entity* map_root = this->get_entity("MapRootEntity");
   Entity* grid_root = this->get_entity("GridRootEntity");

   GraphicalSystem* gs = this->get_system<GraphicalSystem>("GraphicalSystem");
   this->create_fps_display(gs);

   // create selection rect
   Entity* selection_rect = this->create_entity("SelectionRectangleEntity");
   this->add_to_scene_node(hud_root, selection_rect, 0);

   selection_rect->add<Rectangle>("SelectionRectangle", 0, 0, 0, 0);
   selection_rect->get<Rectangle>()->color(Color(66, 108, 167, 175));
   selection_rect->get<Rectangle>()->outline_color(Color(124, 160, 210, 192));
   selection_rect->get<Rectangle>()->outline_thickness(1.f);

   // create tile selection visual
   Entity* tile_selection = this->create_entity("TileSelectionEntity");
   this->add_to_scene_node(grid_root, tile_selection, 0);

   tile_selection->space()->visible(false);

   tile_selection->add<Rectangle>("TileSelectionRectangle", 0, 0, 0, 0);
   tile_selection->get<Rectangle>()->color(Color(255, 255, 255, 128));
   tile_selection->get<Rectangle>()->outline_color(Color(255, 255, 255, 192));
   tile_selection->get<Rectangle>()->outline_thickness(2.0);

   tile_selection->add<Collision>("TileSelectionCollider", sf::FloatRect(0, 0, 0, 0));

   // create tile selection map_root portion
   Entity* tile_selection_maproot = this->create_entity("TileSelectionMapRootEntity");
   this->add_to_scene_node(map_root, tile_selection_maproot, 0);

   tile_selection_maproot->add<Collision>("TileSelectionMapRootCollider", sf::FloatRect(0, 0, 0, 0));

   // this is the window that contains tiles you click on to populate the tilemap
   this->create_tile_palette(gs);
   assert(this->get_entity("TilePalette"));

   // put tile palette window over selection_rect but under mouse cursor
   this->add_to_scene_node(hud_root, this->get_entity("TilePalette"));

   // create menu panel
   Entity* menu_panel = this->get_entity(this->create_panel("Menu", sf::FloatRect(0, 0, game.window().size().x, 30)));
   
   menu_panel->add<PlayerProfile>("MenuPanelPlayerProfile", 1);
   menu_panel->add<Clickable>("MenuPanelClickable");
   menu_panel->add<Collision>("MenuPanelCollision", menu_panel->get<Rectangle>()->local_bounds());
   menu_panel->add<Callback>("MenuPanelCallback", false);
   
   menu_panel->get<Callback>()->camera_resize([menu_panel, gs] () {
      // make menu_panel always take up the entire width of the screen
      float camera_width = gs->camera()->size().x * gs->camera()->viewport().width / gs->camera()->zoom();
      menu_panel->get<Rectangle>()->size(camera_width, menu_panel->get<Rectangle>()->size().y);

      // and don't forget to update the collision volume too
      menu_panel->get<Collision>()->volume(
         sf::FloatRect(
            menu_panel->get<Collision>()->volume().top,
            menu_panel->get<Collision>()->volume().left,
            camera_width,
            menu_panel->get<Collision>()->volume().height
         )
      );
   });
   this->add_to_scene_node(hud_root, menu_panel);

   Entity* file_load_button = this->get_entity(this->create_button("FileLoadButton", sf::FloatRect(0, 0, 0, 30), "File"));
   this->add_to_scene_node(menu_panel, file_load_button);

   file_load_button->get<Callback>()->left_release([this] () {
      this->file_dialog_box();
   });

   sf::FloatRect mms_area(
      file_load_button->space()->position().x + file_load_button->get<Rectangle>()->local_bounds().width,
      0,
      0,
      30
   );

   Entity* mouse_mode_select_button = this->get_entity(this->create_button("MouseModeSelectButton", mms_area, "Tools"));
   this->add_to_scene_node(menu_panel, mouse_mode_select_button);

   mouse_mode_select_button->get<Callback>()->left_release([&game, this] () {
      Entity* menu_panel = this->get_entity("Menu");
      Entity* mouse_mode_select_button = this->get_entity("MouseModeSelectButton");

      sf::Vector2f release_pos;
      release_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      release_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      Entity* dropdown = this->get_entity("MouseModeSelectDropdown");
      if (dropdown) {
         this->remove_entity(dropdown->handle(), true);
      } else {
         dropdown = this->get_entity(
            this->create_panel(
               "MouseModeSelectDropdown",
               sf::FloatRect(
                  mouse_mode_select_button->space()->position().x,
                  menu_panel->space()->position().y + menu_panel->get<Rectangle>()->local_bounds().height,
                  150,
                  100
               )
            )
         );

         dropdown->add<PlayerProfile>("MouseModeSelectDropdownPlayerProfile", 1);
         dropdown->add<Clickable>("MouseModeSelectDropdownClickable");
         dropdown->add<Collision>("MouseModeSelectDropdownCollision", dropdown->get<Rectangle>()->local_bounds());
         dropdown->add<Callback>("MouseModeSelectDropdownCallback", false);
   
         this->add_to_scene_node(menu_panel, dropdown);

         // select tool
         Entity* select_button = this->get_entity(this->create_button("SelectToolButton", sf::FloatRect(0, 10, 150, 30), "Select Tool"));
         this->add_to_scene_node(dropdown, select_button->handle());

         select_button->get<Callback>()->left_release([&game, this] () {
            // TODO: change tile selection color to white?
            this->mouse_script_add_select_behavior(game, this->get_entity_handle("MouseCursorScriptSwappable"));
         });

         // move tool
         Entity* move_button = this->get_entity(this->create_button("MoveToolButton", sf::FloatRect(0, 40, 150, 30), "Move Tool"));
         this->add_to_scene_node(dropdown, move_button->handle());

         move_button->get<Callback>()->left_release([&game, this] () {
            // TODO: change tile selection color to blue?
            // TODO: add select behavior to move behavior so that if you click away it creates a selection, but if you click on the selection, it moves?
            this->mouse_script_add_move_behavior(game, this->get_entity_handle("MouseCursorScriptSwappable"));
         });
      }
   });
}

void BuilderScene::file_dialog_box() {
   Entity* n_box = this->get_entity(this->create_notification(300, 110));

   Entity* n_box_title = this->create_entity("n_box_title");

   n_box_title->add<Text>("n_box_title_text", "Load from File", this->fonts().get("retro"), 12);
   n_box_title->space()->position(10, 10);

   this->add_to_scene_node(n_box, n_box_title);

   Entity* textbox = this->get_entity(this->create_textbox("FileLoadTextbox", 250, 12));
   textbox->space()->position((n_box->get<Rectangle>()->local_bounds().width - textbox->get<Rectangle>()->local_bounds().width) / 2.f, 40);

   this->add_to_scene_node(n_box, textbox);

   Entity* submit_button = this->get_entity(this->create_button("FileLoadSubmitButton", sf::FloatRect(0, 0, 0, 30), "Submit"));
   submit_button->space()->position(
      (n_box->get<Rectangle>()->local_bounds().width - submit_button->get<Rectangle>()->local_bounds().width) / 2.f,
      40 + textbox->get<Rectangle>()->local_bounds().height + 10
   );

   submit_button->get<Callback>()->left_release([this, textbox] () {
      std::string filename = textbox->get<Text>()->string();

      this->remove_entity(this->get_entity_handle("NotificationRootEntity"), true);
      this->load_from_file(filename);
   });

   // do the same thing as left_release if enter is pressed
   submit_button->get<Callback>()->on_update([this, textbox] () {
      InputDevice* keyboard = this->game().input_manager().get_device(1);

      if (keyboard->get("Return")->was_pressed()) {
         std::string filename = textbox->get<Text>()->string();

         this->remove_entity(this->get_entity_handle("NotificationRootEntity"), true);
         this->load_from_file(filename);
      }

      if (keyboard->get("Escape")->was_pressed()) {
         this->remove_entity(this->get_entity_handle("NotificationRootEntity"), true);
      }
   });

   this->add_to_scene_node(n_box, submit_button);
}

void BuilderScene::setup_keybindings(Game& game) {
   Entity* hud_root = this->get_entity("HudRootEntity");
   Entity* map_root = this->get_entity("MapRootEntity");
   Entity* grid_root = this->get_entity("GridRootEntity");
   Entity* grid_entity = this->get_entity("GridEntity");
   Entity* tile_selection = this->get_entity("TileSelectionEntity");
   Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");

   // setup player's input mapping
   game.add_player(1);

   game.get_player(1).bindings().set<MouseXIntent>(0, game.input_manager().get_device(0)->get("PositionX"));
   game.get_player(1).bindings().set<MouseYIntent>(0, game.input_manager().get_device(0)->get("PositionY"));
   game.get_player(1).bindings().set<MouseWheelIntent>(0, game.input_manager().get_device(0)->get("Wheel"));
   game.get_player(1).bindings().set<LeftClickIntent>(0, game.input_manager().get_device(0)->get("Left"));
   game.get_player(1).bindings().set<RightClickIntent>(0, game.input_manager().get_device(0)->get("Right"));

   game.get_player(1).bindings().set<ModalIntent>(0, game.input_manager().get_device(1)->get("LCtrl"));

   game.get_player(1).bindings().set<GridVisibilityToggleIntent>(1, game.input_manager().get_device(1)->get("G"));
   game.get_player(1).bindings().set<VisualDebugIntent>(1, game.input_manager().get_device(1)->get("D"));
   game.get_player(1).bindings().set<ResetViewIntent>(1, game.input_manager().get_device(1)->get("R"));
   game.get_player(1).bindings().set<RemoveTilesIntent>(1, game.input_manager().get_device(1)->get("Delete"));
   game.get_player(1).bindings().set<SerializeMapIntent>(1, game.input_manager().get_device(1)->get("S"));

   game.get_player(1).bindings().set<MoveUpIntent>(1, game.input_manager().get_device(1)->get("Up"));
   game.get_player(1).bindings().set<MoveLeftIntent>(1, game.input_manager().get_device(1)->get("Left"));
   game.get_player(1).bindings().set<MoveRightIntent>(1, game.input_manager().get_device(1)->get("Right"));
   game.get_player(1).bindings().set<MoveDownIntent>(1, game.input_manager().get_device(1)->get("Down"));

   game.get_player(1).bindings().set<UseMoveToolIntent>(1, game.input_manager().get_device(1)->get("C"));

   std::shared_ptr<bool> move_tool_toggle = std::make_shared<bool>(false);

   // add responses to keyboard inputs
   hud_root->add<Callback>("HudRootCallback");
   hud_root->add<PlayerProfile>("HudRootPlayerProfile", 1);

   hud_root->get<Callback>()->on_update([this, grid_root, grid_entity, map_root, tile_selection, tile_selection_maproot, move_tool_toggle] () {
      InputBinding& p1_bindings = this->game().get_player(1).bindings();

      if (p1_bindings.get<GridVisibilityToggleIntent>()->element()->was_pressed() && p1_bindings.get<ModalIntent>()->element()->is_pressed()) {
         this->send_message<SetGridVisibilityMessage>(grid_entity->handle(), this->grid_visible_);
         this->grid_visible_ = !this->grid_visible_;
      }

      if (p1_bindings.get<VisualDebugIntent>()->element()->was_pressed() && p1_bindings.get<ModalIntent>()->element()->is_pressed()) {
         this->send_message<SetVisualDebugMessage>(this->visual_debug_enable_);
         this->visual_debug_enable_ = !this->visual_debug_enable_;
      }

      if (p1_bindings.get<ResetViewIntent>()->element()->was_pressed() && p1_bindings.get<ModalIntent>()->element()->is_pressed()) {
         map_root->space()->position(0, 30); // reset to point which is not obscured by menubar
         map_root->space()->scale(sf::Vector2f(1.f, 1.f));

         sf::Vector2f pos = tile_selection->space()->position();
         sf::Vector2f end = pos + tile_selection->get<Rectangle>()->size();

         sf::Vector2i pos_idx = grid_entity->get<Grid>()->grid_index(pos);
         sf::Vector2i end_idx = grid_entity->get<Grid>()->grid_index(end);

         // reset grid
         grid_root->space()->position(0, 30); // reset to point which is not obscured by menubar
         grid_entity->get<Grid>()->zoom_factor(1.f, 1.f);

         // recalculate tile selection visual
         sf::Vector2f new_pos;
         new_pos.x = grid_entity->get<Grid>()->tile_width() * pos_idx.x;
         new_pos.y = grid_entity->get<Grid>()->tile_height() * pos_idx.y;

         sf::Vector2f new_end;
         new_end.x = grid_entity->get<Grid>()->tile_width() * end_idx.x;
         new_end.y = grid_entity->get<Grid>()->tile_height() * end_idx.y;

         tile_selection->space()->position(new_pos);
         tile_selection->get<Rectangle>()->size(new_end - new_pos);

         tile_selection->get<Collision>()->volume(sf::FloatRect(sf::Vector2f(0, 0), new_end - new_pos));

         // remove tile popup
         Entity* tile_popup = this->get_entity("TilePopup");
         if (tile_popup) {
            this->remove_entity(tile_popup->handle(), true);
         }
      }

      if (p1_bindings.get<MoveUpIntent>()->element()->is_pressed()) {
         map_root->space()->move(sf::Vector2f(0, 10));
         grid_root->space()->move(sf::Vector2f(0, 10));
      }

      if (p1_bindings.get<MoveLeftIntent>()->element()->is_pressed()) {
         map_root->space()->move(sf::Vector2f(10, 0));
         grid_root->space()->move(sf::Vector2f(10, 0));
      }

      if (p1_bindings.get<MoveRightIntent>()->element()->is_pressed()) {
         map_root->space()->move(sf::Vector2f(-10, 0));
         grid_root->space()->move(sf::Vector2f(-10, 0));
      }

      if (p1_bindings.get<MoveDownIntent>()->element()->is_pressed()) {
         map_root->space()->move(sf::Vector2f(0, -10));
         grid_root->space()->move(sf::Vector2f(0, -10));
      }

      if (p1_bindings.get<RemoveTilesIntent>()->element()->was_pressed()) {
         if (map_root && tile_selection_maproot) {
            TileMap* map = map_root->get<TileMap>();
            Collision* collision = tile_selection_maproot->get<Collision>();

            if (map && collision) {
               map->remove(
                  // don't use global transform (need translation but not zoom from map_root)
                  tile_selection_maproot->space()->transform().transformRect(
                     tile_selection_maproot->get<Collision>()->volume()
                  )
               );
            }
         }
      }

      if (p1_bindings.get<SerializeMapIntent>()->element()->was_pressed() && p1_bindings.get<ModalIntent>()->element()->is_pressed()) {
         if (this->scene_data_filename == "") {
            return;
         }

         FileChannel* fc = new FileChannel(this->scene_data_filename);
         Serializer* serializer = new JSONSerializer(3);

         fc->seek(0);
         std::string old_file_contents = serializer->read(static_cast<Channel&>(*fc));

         // TODO: this should go in scene serialize/deserialize function?
         Serializer::SerialData scene_data = serializer->deserialize(*this, old_file_contents);

         // modify the tilemap0 entry
         scene_data["Tilemap0"] = map_root->get<TileMap>()->serialize(*serializer);

         // write back to file
         fc->remove();
         fc->seek(0);
         fc->send(serializer->serialize(scene_data));

         Game::logger().msg("BuilderSceneInputSystem", Logger::INFO, "Saving map to file '" + fc->filename());

         delete serializer;
      }

      if (this->game().input_manager().get_device(1)->get("F")->was_pressed() && p1_bindings.get<ModalIntent>()->element()->is_pressed()) {
         Entity* notification_root = this->get_entity("NotificationRootEntity");
         if (!notification_root) {
            this->file_dialog_box();
         }
      }

      // move tool on press and hold
      if (p1_bindings.get<ModalIntent>()->element()->is_pressed()) {
         if (p1_bindings.get<UseMoveToolIntent>()->element()->was_pressed()) {
            // set move tool behavior
            this->mouse_script_add_move_behavior(this->game(), this->get_entity_handle("MouseCursorScriptSwappable"));
         } else if (p1_bindings.get<UseMoveToolIntent>()->element()->was_released()) {
            Entity* mouse_cursor_script = this->get_entity("MouseCursorScriptSwappable");
            mouse_cursor_script->get<Callback>()->left_release();

            // set select tool behavior
            this->mouse_script_add_select_behavior(this->game(), this->get_entity_handle("MouseCursorScriptSwappable"));
         }
      } else {
         // move tool toggle
         if (p1_bindings.get<UseMoveToolIntent>()->element()->was_pressed() && !(*move_tool_toggle)) {
            // set move tool behavior
            this->mouse_script_add_move_behavior(this->game(), this->get_entity_handle("MouseCursorScriptSwappable"));
            *move_tool_toggle = true;
         } else if (p1_bindings.get<UseMoveToolIntent>()->element()->was_pressed() && *move_tool_toggle) {
            Entity* mouse_cursor_script = this->get_entity("MouseCursorScriptSwappable");
            mouse_cursor_script->get<Callback>()->left_release();

            // set select tool behavior
            this->mouse_script_add_select_behavior(this->game(), this->get_entity_handle("MouseCursorScriptSwappable"));
            *move_tool_toggle = false;
         }
      }

      if (p1_bindings.get<ModalIntent>()->element()->was_released() && p1_bindings.get<UseMoveToolIntent>()->element()->is_pressed()) {
            Entity* mouse_cursor_script = this->get_entity("MouseCursorScriptSwappable");
            mouse_cursor_script->get<Callback>()->left_release();

            this->mouse_script_add_select_behavior(this->game(), this->get_entity_handle("MouseCursorScriptSwappable"));
      }
   });
}

void BuilderScene::load_from_file(std::string filename) {
   Entity* map_root = this->get_entity("MapRootEntity");
   Entity* grid_entity = this->get_entity("GridEntity");

   assert(map_root && grid_entity);

   // load or create a tile map
   JSONSerializer serializer;
   FileChannel save_file(filename);

   // TODO: loading scene data should be in scene's serialization/deserialization?
   save_file.seek(0);
   std::string raw_save_file = serializer.read(save_file);

   if (raw_save_file == "") {
      Entity* n_box = this->get_entity(this->create_notification(300, 110));
      Entity* error_message = this->create_entity("NotificationErrorMsg");

      this->add_to_scene_node(n_box, error_message);

      error_message->add<Text>("NotificationErrorMsgText", "ERROR: Invalid filename\nspecified!", this->fonts().get("retro"), 12);
      error_message->get<Text>()->color(Color(188, 45, 69, 255));

      error_message->space()->position(
         (n_box->get<Rectangle>()->local_bounds().width - error_message->get<Text>()->local_bounds().width) / 2.f,
         20
      );

      Entity* ok_button = this->get_entity(this->create_button("NotificationErrorButton", sf::FloatRect(0, 0, 0, 30), "OK"));
      ok_button->space()->position(
         (n_box->get<Rectangle>()->local_bounds().width - ok_button->get<Rectangle>()->local_bounds().width) / 2.f,
         20 + error_message->get<Text>()->local_bounds().height + 20
      );

      ok_button->get<Callback>()->left_release([this] () {
         this->remove_entity(this->get_entity_handle("NotificationRootEntity"), true);
      });

      // do the same thing as left_release if enter is pressed
      ok_button->get<Callback>()->on_update([this] () {
         InputDevice* keyboard = this->game().input_manager().get_device(1);

         if (keyboard->get("Return")->was_pressed()) {
            this->remove_entity(this->get_entity_handle("NotificationRootEntity"), true);
         }
      });

      this->add_to_scene_node(n_box, ok_button);

      return;
   }

   // TODO: catch parse errors
   Serializer::SerialData scene_data = serializer.deserialize(*this, raw_save_file);

   this->scene_data_filename = filename;

   // load in map textures
   Serializer::SerialData texture_data = serializer.deserialize(*this, scene_data["Textures"]);
   for (Serializer::SerialData::iterator it = texture_data.begin(); it != texture_data.end(); ++it) {
      Serializer::SerialData texture_item = serializer.deserialize(*this, it->second);
      this->textures().load(texture_item["id"], texture_item["filename"]);
   }
   Game::logger().msg(this->id(), Logger::INFO, this->textures());

   // deserialize tilemap
   if (scene_data.find("Tilemap0") != scene_data.end()) {
      map_root->get<TileMap>()->deserialize(static_cast<Serializer&>(serializer), *this, scene_data["Tilemap0"]);
   }

   // deserialize grid
   if (scene_data.find("Grid0") != scene_data.end()) {
      grid_entity->get<Grid>()->deserialize(static_cast<Serializer&>(serializer), *this, scene_data["Grid0"]);
   }

   Serializer::SerialData tileset_data = serializer.deserialize(*this, scene_data["Tileset0"]);

   // add tile textures to the tile palette window
   Entity* tile_palette = this->get_entity("TilePalette");
   Entity* tile_palette_layer1 = this->get_entity("TilePaletteLayer1");
   Entity* tile_palette_hover = this->get_entity("TilePaletteHover");
   Entity* tile_palette_outline = this->get_entity("TilePaletteDecoration");
   assert(tile_palette && tile_palette_layer1 && tile_palette_hover);

   int tile_scale_factor = std::round(64 / std::max(grid_entity->get<Grid>()->tile_width(), grid_entity->get<Grid>()->tile_height()));
   int tiles_per_row = tile_palette->get<Rectangle>()->size().x / (tile_scale_factor * grid_entity->get<Grid>()->tile_width());
   int side_padding = (tile_palette->get<Rectangle>()->size().x - (tile_scale_factor * grid_entity->get<Grid>()->tile_width() * tiles_per_row)) / 2;
   int top_padding = 20;
   int bottom_padding = 10;

   // resize tile palette to be tall enough
   int tile_palette_height = top_padding + bottom_padding + (tile_scale_factor * grid_entity->get<Grid>()->tile_height()) * std::ceil(std::stoi(tileset_data["num_tiles"]) / static_cast<float>(tiles_per_row));
   tile_palette->get<Rectangle>()->size(tile_palette->get<Rectangle>()->size().x, tile_palette_height);
   tile_palette_outline->get<Rectangle>()->size(tile_palette_outline->get<Rectangle>()->size().x, tile_palette_height - 15);

   for (int tile_idx = 0; tile_idx < std::stoi(tileset_data["num_tiles"]); ++tile_idx) {
      // TODO: move this to some sort of tileset object?
      Serializer::SerialData tile_data = serializer.deserialize(*this, tileset_data["tile_" + std::to_string(tile_idx)]);

      sf::IntRect tile_texture_rect(
         std::stoi(tile_data["left"]),
         std::stoi(tile_data["top"]),
         std::stoi(tile_data["width"]),
         std::stoi(tile_data["height"])
      );

      sf::Vector2f tile_texture_pos(
         (tile_idx % tiles_per_row) * tile_scale_factor * grid_entity->get<Grid>()->tile_width() + side_padding,
         (tile_idx / tiles_per_row) * tile_scale_factor * grid_entity->get<Grid>()->tile_height() + top_padding
      );

      Entity* entity = this->create_entity(tile_data["id"] + "_tile_palette_button");
      this->add_to_scene_node(tile_palette_layer1, entity);

      entity->space()->position(tile_texture_pos);

      // scale tile sprites to normalize them so it's easy to look at and click on
      entity->add<Sprite>(tile_data["id"] + "_sprite", this->textures().get(tileset_data["id"]), tile_texture_rect);
      entity->get<Sprite>()->scale(tile_scale_factor, tile_scale_factor);

      entity->add<PlayerProfile>(tile_data["id"] + "_playerProfile", 1);
      entity->add<Clickable>(tile_data["id"] + "_clickable");
      entity->add<Collision>(tile_data["id"] + "_collision", entity->get<Sprite>()->global_bounds());

      // define texture button behavior
      entity->add<Callback>(tile_data["id"] + "_callback");
      entity->get<Callback>()->mouse_in([entity, tile_palette_hover] () {
         tile_palette_hover->space()->position(entity->space()->position());
         tile_palette_hover->space()->visible(true);
      });

      entity->get<Callback>()->mouse_out([entity, tile_palette_hover] () {
         tile_palette_hover->space()->visible(false);
      });

      entity->get<Callback>()->left_release([this, entity, map_root, grid_entity] () {
         Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");
         assert(tile_selection_maproot);

         sf::FloatRect tsc = tile_selection_maproot->get<Collision>()->volume();
         tsc.left += tile_selection_maproot->space()->position().x;
         tsc.top += tile_selection_maproot->space()->position().y;

         // on click, fill the selected area (if it is active) with the clicked on tile
         for (int x_pos = tsc.left; x_pos < tsc.left + tsc.width; x_pos += grid_entity->get<Grid>()->tile_width()) {
            for (int y_pos = tsc.top; y_pos < tsc.top + tsc.height; y_pos += grid_entity->get<Grid>()->tile_height()) {
               Sprite sprite(*entity->get<Sprite>());
               sprite.offset(x_pos, y_pos);
               sprite.scale(1.f, 1.f); // reset any scaling done on palette tiles

               map_root->get<TileMap>()->set(sprite);
            }
         }
      });
   }

   // resize tile palette hover entity to match the deserialized grid
   tile_palette_hover->get<Rectangle>()->size(static_cast<float>(tile_scale_factor) * sf::Vector2f(grid_entity->get<Grid>()->tile_width(), grid_entity->get<Grid>()->tile_height()));

   // refresh the grid, it won't refresh since the camera hasn't changed
   // (this might not be the best solution)
   GridSystem* grid_system = this->get_system<GridSystem>("GridSystem");
   grid_system->force_update(); 
}

void BuilderScene::create_mouse_entity(Game& game) {
   Entity* hud_root = this->get_entity("HudRootEntity");
   Entity* map_root = this->get_entity("MapRootEntity");

   // create mouse cursor (this adds to scene graph root by default, which will be on top of everything)
   Entity* mouse_cursor = this->create_entity("MouseCursorEntity");

   mouse_cursor->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   mouse_cursor->add<Sprite>("MouseCursorSprite", this->textures().get("cursors"), sf::IntRect(0, 0, 4, 4));
   mouse_cursor->get<Sprite>()->scale(3, 3);

   mouse_cursor->add<Text>("MouseCursorText", "0, 0", this->fonts().get("retro"), 12);
   mouse_cursor->get<Text>()->offset(0, 18);
   mouse_cursor->get<Text>()->color(Color(sf::Color::White));

   mouse_cursor->add<Callback>("MouseCursorCallback");
   mouse_cursor->get<Callback>()->mouse_move([mouse_cursor, map_root, this] () {
      sf::Vector2f new_pos;
      new_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      sf::Vector2f screen_pos = map_root->space()->inverse_transform().transformPoint(new_pos);

      // move mouse cursor
      mouse_cursor->space()->position(new_pos);

      // update mouse cursor text
      mouse_cursor->get<Text>()->string(
         std::to_string(static_cast<int>(new_pos.x)) +
         ", " +
         std::to_string(static_cast<int>(new_pos.y)) + "\n" +
         std::to_string(static_cast<int>(screen_pos.x)) +
         ", " +
         std::to_string(static_cast<int>(screen_pos.y))
      );
   });

   Entity* mouse_cursor_script = this->create_entity("MouseCursorScriptEntity");

   mouse_cursor_script->add<PlayerProfile>("MouseCursorPlayerProfile", 1);
   mouse_cursor_script->add<Clickable>("MouseCursorClickable");
   mouse_cursor_script->add<Collision>("MouseCursorCollision", sf::FloatRect(0, 0, game.window().size().x, game.window().size().y));
   mouse_cursor_script->add<Callback>("MouseCursorScriptCallback");

   this->add_to_scene_node(hud_root, mouse_cursor_script, 0);

   // put pan and zoom behavior on the parent mouse cursor_script and then have a child
   // script entity that will be swapped out with different tool behaviors
   Entity* mcs_swappable = this->create_entity("MouseCursorScriptSwappable");

   mcs_swappable->add<PlayerProfile>("MouseCursorSwappablePlayerProfile", 1);
   mcs_swappable->add<Clickable>("MouseCursorSwappableClickable");
   mcs_swappable->add<Collision>("MouseCursorSwappableCollision", sf::FloatRect(0, 0, game.window().size().x, game.window().size().y));
   mcs_swappable->add<Callback>("MouseCursorSwappableScriptCallback");

   this->add_to_scene_node(mouse_cursor_script, mcs_swappable);

   this->mouse_script_add_zoom_behavior(game, mouse_cursor_script->handle());
   this->mouse_script_add_pan_behavior(game, mouse_cursor_script->handle());
   this->mouse_script_add_select_behavior(game, mcs_swappable->handle());
}

void BuilderScene::create_backdrop(GraphicalSystem* gs) {
   Entity* backdrop = this->create_entity("Backdrop Entity");
   this->add_to_scene_node(this->scene_graph(), backdrop, 0);
   
   backdrop->add<VertexList>("BackdropVertexList", sf::TrianglesStrip, 4);
   backdrop->get<VertexList>()->vertex_color(0, Color(50, 50, 50, 255));
   backdrop->get<VertexList>()->vertex_color(1, Color(25, 25, 25, 255));
   backdrop->get<VertexList>()->vertex_color(2, Color(50, 50, 50, 255));
   backdrop->get<VertexList>()->vertex_color(3, Color(25, 25, 25, 255));

   backdrop->add<PlayerProfile>("BackdropPlayerProfile", 1);
   backdrop->add<Callback>("BackdropCallback");

   backdrop->get<Callback>()->camera_resize([backdrop, gs] () {
      sf::Vector2f camera_bounds = gs->camera()->size();

      camera_bounds.x *= gs->camera()->viewport().width / gs->camera()->zoom();
      camera_bounds.y *= gs->camera()->viewport().height / gs->camera()->zoom();

      backdrop->get<VertexList>()->vertex_position(0, sf::Vector2f(0, 0));
      backdrop->get<VertexList>()->vertex_position(1, sf::Vector2f(0, camera_bounds.y));
      backdrop->get<VertexList>()->vertex_position(2, sf::Vector2f(camera_bounds.x, 0));
      backdrop->get<VertexList>()->vertex_position(3, sf::Vector2f(camera_bounds.x, camera_bounds.y));
   });
}

void BuilderScene::create_fps_display(GraphicalSystem* gs) {
   Entity* hud_root = this->get_entity("HudRootEntity");
   assert(hud_root);

   Entity* fps_display = this->create_entity("FPS Display Entity");
   this->add_to_scene_node(hud_root, fps_display);

   fps_display->add<Text>("FPS Display Text", "FPS: ??", this->fonts().get("retro"), 12);
   fps_display->get<Text>()->color(Color(216, 138, 0, 255));

   fps_display->add<PlayerProfile>("FPSDisplayPlayerProfile", 1);
   fps_display->add<Callback>("FPSDisplayCallback");

   fps_display->get<Callback>()->camera_resize([fps_display, gs] () {
      sf::Vector2f camera_bounds = gs->camera()->size();
      sf::Vector2f text_size;

      text_size.x = fps_display->get<Text>()->local_bounds().width;
      text_size.y = fps_display->get<Text>()->local_bounds().height;

      camera_bounds.x *= gs->camera()->viewport().width / gs->camera()->zoom();
      camera_bounds.y *= gs->camera()->viewport().height / gs->camera()->zoom();

      fps_display->space()->position(camera_bounds - text_size - sf::Vector2f(10, 10));
   });

   fps_display->get<Callback>()->on_update([this, fps_display, gs] () {
      // update fps read
      if (!this->frame_count) {
         this->last_frame_time = (((float)this->frame_measurement_interval / this->clock.getElapsedTime().asSeconds()) * this->game().settings.framerate_smoothing())
                                 + (this->last_frame_time * (1.0 - this->game().settings.framerate_smoothing()));
         this->clock.restart();

         fps_display->get<Text>()->string("FPS: " + std::to_string(this->last_frame_time));
      }
      this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
   });
}

void BuilderScene::create_tile_palette(GraphicalSystem* gs) {
   Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");
   Entity* map_root = this->get_entity("MapRootEntity");
   Entity* grid_root = this->get_entity("GridRootEntity");
   Entity* grid_entity = this->get_entity("GridEntity");

   assert(tile_selection_maproot);
   assert(map_root);
   assert(grid_root);
   assert(grid_entity);

   // create hover sprite
   Entity* tile_palette_hover = this->create_entity("TilePaletteHover");

   tile_palette_hover->space()->visible(false);
   tile_palette_hover->add<Rectangle>("TilePaletteRectangle", 0, 0, grid_entity->get<Grid>()->tile_width(), grid_entity->get<Grid>()->tile_height());
   tile_palette_hover->get<Rectangle>()->color(Color(255, 255, 255, 100));
   tile_palette_hover->get<Rectangle>()->outline_color(Color(108, 46, 167, 100));
   tile_palette_hover->get<Rectangle>()->outline_thickness(2.f);

   // create tile palette
   Entity* tile_palette = this->get_entity(this->create_panel("TilePalette", sf::FloatRect(0, 0, 220, 480), true, "Tileset"));

   tile_palette->add<PlayerProfile>("MouseCursorPlayerProfile", 1);
   tile_palette->add<Clickable>("TilePaletteClickable");
   tile_palette->add<Collision>("TilePaletteCollision", tile_palette->get<Rectangle>()->local_bounds());

   tile_palette->add<Callback>("TilePaletteCallback", false);

   // readjust tile palette window (and children) when the camera is resized
   tile_palette->get<Callback>()->camera_resize([tile_palette, gs] () {
      float camera_width = gs->camera()->size().x * gs->camera()->viewport().width / gs->camera()->zoom();
      sf::Vector2f new_pos(camera_width - tile_palette->get<Rectangle>()->size().x - 10, 40);
      sf::Vector2f old_pos(tile_palette->space()->position());

      // move to the upper right corner
      tile_palette->space()->move(new_pos - old_pos);
   });

   Entity* tile_palette_layer_1 = this->create_entity("TilePaletteLayer1");
   this->add_to_scene_node(tile_palette, tile_palette_layer_1);

   // putting this on top of layer 1 (tiles are on this layer)
   this->add_to_scene_node(tile_palette, tile_palette_hover);
}

void BuilderScene::mouse_script_add_zoom_behavior(Game& game, Handle mouse_entity) {
   Entity* mouse_cursor_script = this->get_entity(mouse_entity);
   if (!mouse_cursor_script) {
      Game::logger().msg(this->id(), Logger::WARNING, "(mouse_script_add_zoom_behavior) Mouse cursor script entity not found.");
      return;
   }

   mouse_cursor_script->get<Callback>()->camera_resize([mouse_cursor_script, this] () {
      GraphicalSystem* gs = this->get_system<GraphicalSystem>("GraphicalSystem");

      sf::Vector2f new_size = gs->camera()->size();
      new_size.x *= gs->camera()->viewport().width;
      new_size.y *= gs->camera()->viewport().height;

      // make sure the collision volume fills the whole camera
      mouse_cursor_script->get<Collision>()->volume(sf::Vector2f(0, 0), new_size);
   });

   mouse_cursor_script->get<Callback>()->mouse_wheel([mouse_cursor_script, this] () {
      Entity* tile_selection = this->get_entity("TileSelectionEntity");
      Entity* map_root = this->get_entity("MapRootEntity");
      Entity* grid_entity = this->get_entity("GridEntity");

      float mouse_wheel_pos = this->game().get_player(1).bindings().get<MouseWheelIntent>()->element()->position();
      float wheel_delta = mouse_wheel_pos - mouse_cursor_script->get<Callback>()->prev_mouse_wheel_pos();

      sf::Vector2f new_scale(map_root->space()->scale() + sf::Vector2f(wheel_delta / 12.5f, wheel_delta / 12.5f));
      new_scale.x = std::max(new_scale.x, 0.25f);
      new_scale.y = std::max(new_scale.y, 0.25f);

      Entity* tile_popup = this->get_entity("TilePopup");
      Entity* grid_root = this->get_entity("GridRootEntity");
      sf::Vector2i pos_idx;

      if (tile_popup) {
         // need to get original grid index for tile popup adjustment
         sf::Vector2f pos = tile_popup->space()->position();
         pos = grid_root->space()->inverse_transform().transformPoint(pos);
         pos_idx = grid_entity->get<Grid>()->grid_index(grid_entity->get<Grid>()->floor(pos));
      }

      map_root->space()->scale(new_scale);

      // adjust tile selection visual on the grid layer
      if (tile_selection->space()->visible()) {
         sf::Vector2f pos = tile_selection->space()->position();
         sf::Vector2f end = pos + tile_selection->get<Rectangle>()->size();

         sf::Vector2i pos_idx = grid_entity->get<Grid>()->grid_index(pos);
         sf::Vector2i end_idx = grid_entity->get<Grid>()->grid_index(end);

         sf::Vector2f new_pos;
         new_pos.x = grid_entity->get<Grid>()->tile_width() * pos_idx.x * new_scale.x;
         new_pos.y = grid_entity->get<Grid>()->tile_height() * pos_idx.y * new_scale.y;

         sf::Vector2f new_end;
         new_end.x = grid_entity->get<Grid>()->tile_width() * end_idx.x * new_scale.x;
         new_end.y = grid_entity->get<Grid>()->tile_height() * end_idx.y * new_scale.y;

         tile_selection->space()->position(new_pos);
         tile_selection->get<Rectangle>()->size(new_end - new_pos);

         tile_selection->get<Collision>()->volume(tile_selection->get<Rectangle>()->global_bounds());
      }

      grid_entity->get<Grid>()->zoom_factor(new_scale);

      // adjust tile popup if exists
      if (tile_popup) {
         Entity* tile_popup_cursor = this->get_entity("TilePopupCursor");

         sf::Vector2f new_pos;
         new_pos.x = grid_entity->get<Grid>()->tile_width() * pos_idx.x * new_scale.x;
         new_pos.y = grid_entity->get<Grid>()->tile_height() * pos_idx.y * new_scale.y;

         new_pos = grid_root->space()->transform().transformPoint(new_pos);

         tile_popup->space()->position(new_pos + sf::Vector2f(10, 5));

         tile_popup_cursor->get<Rectangle>()->size(grid_entity->get<Grid>()->tile_width() * new_scale.x, grid_entity->get<Grid>()->tile_height() * new_scale.y);
         tile_popup_cursor->get<Collision>()->volume(tile_popup_cursor->get<Rectangle>()->local_bounds());

         // I'm not sure how this is working... (offset calculation is VERY messy)
         tile_popup_cursor->space()->position(sf::Vector2f(-1 * grid_entity->get<Grid>()->tile_width() * new_scale.x, 0) - sf::Vector2f(10, 5));
      }
   });
}

void BuilderScene::mouse_script_add_pan_behavior(Game& game, Handle mouse_entity) {
   Entity* mouse_cursor_script = this->get_entity(mouse_entity);
   if (!mouse_cursor_script) {
      Game::logger().msg(this->id(), Logger::WARNING, "(mouse_script_add_pan_behavior) Mouse cursor script entity not found.");
      return;
   }

   mouse_cursor_script->get<Callback>()->camera_resize([mouse_cursor_script, this] () {
      GraphicalSystem* gs = this->get_system<GraphicalSystem>("GraphicalSystem");

      sf::Vector2f new_size = gs->camera()->size();
      new_size.x *= gs->camera()->viewport().width;
      new_size.y *= gs->camera()->viewport().height;

      // make sure the collision volume fills the whole camera
      mouse_cursor_script->get<Collision>()->volume(sf::Vector2f(0, 0), new_size);
   });

   mouse_cursor_script->get<Callback>()->mouse_move([mouse_cursor_script, this] () {
      Entity* map_root = this->get_entity("MapRootEntity");
      Entity* grid_root = this->get_entity("GridRootEntity");

      Clickable* clickable = mouse_cursor_script->get<Clickable>();
      assert (clickable);

      sf::Vector2f new_pos;
      new_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // pan map camera if right click is down
      Callback* callback = mouse_cursor_script->get<Callback>();
      if (clickable && callback && clickable->is_right_clicked()) {
         map_root->space()->move(new_pos - callback->prev_mouse_pos());
         grid_root->space()->move(new_pos - callback->prev_mouse_pos());

         Entity* tile_popup = this->get_entity("TilePopup");
         if (tile_popup) {
            tile_popup->space()->move(new_pos - callback->prev_mouse_pos());
         }
      }
   });

   mouse_cursor_script->get<Callback>()->left_release([mouse_cursor_script, this] () {
      Entity* mouse_mode_select_dropdown = this->get_entity("MouseModeSelectDropdown");

      // TODO: not sure where to put this code...
      if (mouse_mode_select_dropdown) {
         this->remove_entity(mouse_mode_select_dropdown->handle(), true);
      }
   });

   // TODO: this should go in a "tile_inspector_behavior function"
   mouse_cursor_script->get<Callback>()->right_release([mouse_cursor_script, this] () {
      Entity* map_root = this->get_entity("MapRootEntity");
      Entity* hud_root = this->get_entity("HudRootEntity");
      Entity* grid_root = this->get_entity("GridRootEntity");
      Entity* grid_entity = this->get_entity("GridEntity");
      Entity* selection_rect = this->get_entity("SelectionRectangleEntity");
      Entity* tile_selection = this->get_entity("TileSelectionEntity");
      Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");
      Entity* mouse_mode_select_dropdown = this->get_entity("MouseModeSelectDropdown");

      sf::Vector2f hud_click_pos;
      hud_click_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      hud_click_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // TODO: not sure where to put this code...
      if (mouse_mode_select_dropdown) {
         this->remove_entity(mouse_mode_select_dropdown->handle(), true);
      }

      // need to set scale to 1 temporarily to get inverse transform for tilemap search...
      sf::Vector2f map_scale = map_root->space()->scale();
      map_root->space()->scale(1.f, 1.f);

      sf::Transform map_inverse_transform = map_root->space()->inverse_transform();
      map_root->space()->scale(map_scale);

      sf::Vector2f release_pos = map_inverse_transform.transformPoint(hud_click_pos);

      Clickable* clickable = mouse_cursor_script->get<Clickable>();
      assert (clickable);

      sf::Vector2f ts_origin = grid_root->space()->inverse_transform().transformPoint(clickable->right_click_pos());

      // rectangle calculation
      sf::Vector2f pos;
      sf::Vector2f end;

      pos.x = std::min(ts_origin.x, release_pos.x);
      pos.y = std::min(ts_origin.y, release_pos.y);
      
      end.x = std::max(ts_origin.x, release_pos.x);
      end.y = std::max(ts_origin.y, release_pos.y);

      // get rid of selection rect
      selection_rect->space()->visible(false);

      // update tile selection
      bool is_drag_gesture = (end.x - pos.x >= grid_entity->get<Grid>()->tile_width() / 3.f ||
                              end.y - pos.y >= grid_entity->get<Grid>()->tile_height() / 3.f);

      Entity* tile_popup = this->get_entity("TilePopup");
      Entity* tile_popup_cursor = this->get_entity("TilePopupCursor");

      float tile_width = grid_entity->get<Grid>()->tile_width() * grid_entity->get<Grid>()->zoom_factor().x;
      float tile_height = grid_entity->get<Grid>()->tile_height() * grid_entity->get<Grid>()->zoom_factor().y;

      if (!is_drag_gesture) {
         // if the tile popup is already up, remove it
         if (tile_popup && tile_popup_cursor) {
            this->remove_entity(tile_popup->handle(), true);
            return;
         }

         sf::Vector2i clicked_tile = grid_entity->get<Grid>()->grid_index(grid_entity->get<Grid>()->floor(release_pos));
         sf::FloatRect search_area(
            clicked_tile.x * grid_entity->get<Grid>()->tile_width(),
            clicked_tile.y * grid_entity->get<Grid>()->tile_height(),
            grid_entity->get<Grid>()->tile_width(),
            grid_entity->get<Grid>()->tile_height()
         );
         std::vector<TileMap::TileType*> tiles = map_root->get<TileMap>()->find(search_area);

         if (tiles.size() > 0) {
            sf::Vector2f panel_origin(clicked_tile.x * tile_width, clicked_tile.y * tile_height);
            panel_origin = grid_root->space()->transform().transformPoint(panel_origin);

            sf::FloatRect panel_pos(
               panel_origin.x + tile_width + 10,
               panel_origin.y + 5, 
               64 + 20,
               tiles.size() * 64 + 30
            );

            if (tile_popup) {
               this->remove_entity(tile_popup->handle(), true);
            }

            tile_popup = this->get_entity(this->create_panel("TilePopup", panel_pos, true, "Tiles"));
            this->add_to_scene_node(hud_root, tile_popup, 1);

            tile_popup->add<PlayerProfile>("TilePopupPlayerProfile", 1);
            tile_popup->add<Clickable>("TilePopupClickable");
            tile_popup->add<Collision>("TilePopupCollision", tile_popup->get<Rectangle>()->local_bounds());
            tile_popup->add<Callback>("TilePopupCallback", false);

            unsigned int layer_idx = 0;
            for (std::vector<TileMap::TileType*>::const_reverse_iterator it = tiles.rbegin(); it != tiles.rend(); ++it) {
               Entity* t = this->create_entity("tile_layer_" + std::to_string(layer_idx));

               if ((*it)->type() == typeid(Circle)) {
                  t->add<Circle>(boost::get<Circle>(**it));
                  t->get<Circle>()->offset(0, 0);

                  sf::Vector2f new_scale;
                  new_scale.x = 64 / t->get<Circle>()->local_bounds().width * t->get<Circle>()->scale().x;
                  new_scale.y = 64 / t->get<Circle>()->local_bounds().height * t->get<Circle>()->scale().y;

                  t->get<Circle>()->scale(new_scale);
               } else if ((*it)->type() == typeid(Rectangle)) {
                  t->add<Rectangle>(boost::get<Rectangle>(**it));
                  t->get<Rectangle>()->offset(0, 0);

                  sf::Vector2f new_scale;
                  new_scale.x = 64 / t->get<Rectangle>()->local_bounds().width * t->get<Rectangle>()->scale().x;
                  new_scale.y = 64 / t->get<Rectangle>()->local_bounds().height * t->get<Rectangle>()->scale().y;

                  t->get<Rectangle>()->scale(new_scale);
               } else if ((*it)->type() == typeid(Sprite)) {
                  t->add<Sprite>(boost::get<Sprite>(**it));
                  t->get<Sprite>()->offset(0, 0);

                  sf::Vector2f new_scale;
                  new_scale.x = 64 / t->get<Sprite>()->local_bounds().width * t->get<Sprite>()->scale().x;
                  new_scale.y = 64 / t->get<Sprite>()->local_bounds().height * t->get<Sprite>()->scale().y;

                  t->get<Sprite>()->scale(new_scale);
               } else if ((*it)->type() == typeid(Text)) {
                  t->add<Text>(boost::get<Text>(**it));
                  t->get<Text>()->offset(0, 0);

                  sf::Vector2f new_scale;
                  new_scale.x = 64 / t->get<Text>()->local_bounds().width * t->get<Text>()->scale().x;
                  new_scale.y = 64 / t->get<Text>()->local_bounds().height * t->get<Text>()->scale().y;

                  t->get<Text>()->scale(new_scale);
               } else if ((*it)->type() == typeid(VertexList)) {
                  t->add<VertexList>(boost::get<VertexList>(**it));
                  t->get<VertexList>()->offset(0, 0);

                  sf::Vector2f new_scale;
                  new_scale.x = 64 / t->get<VertexList>()->local_bounds().width * t->get<VertexList>()->scale().x;
                  new_scale.y = 64 / t->get<VertexList>()->local_bounds().height * t->get<VertexList>()->scale().y;

                  t->get<VertexList>()->scale(new_scale);
               }

               t->space()->position(10, 64 * layer_idx + 20);

               this->add_to_scene_node(tile_popup, t);

               // add x button
               Entity* delete_button = this->create_entity("DeleteLayer" + std::to_string(layer_idx));

               delete_button->add<Sprite>("DeleteLayer" + std::to_string(layer_idx) + "Sprite", this->textures().get("ui_close_button"));
               delete_button->get<Sprite>()->scale(2.f, 2.f);

               delete_button->add<PlayerProfile>("DeleteLayer" + std::to_string(layer_idx) + "PlayerProfile", 1);
               delete_button->add<Clickable>("DeleteLayer" + std::to_string(layer_idx) + "Clickable");
               delete_button->add<Collision>("DeleteLayer" + std::to_string(layer_idx) + "Collision", delete_button->get<Sprite>()->global_bounds());

               delete_button->add<Callback>("DeleteLayer" + std::to_string(layer_idx) + "Callback");

               delete_button->get<Callback>()->mouse_in([delete_button, this] () {
                  delete_button->get<Sprite>()->color(sf::Color(255, 255, 255, 127));
               });

               delete_button->get<Callback>()->mouse_out([delete_button, this] () {
                  delete_button->get<Sprite>()->color(sf::Color(255, 255, 255, 255));
               });

               auto tile_ref = *it;
               delete_button->get<Callback>()->left_release([delete_button, tile_ref, t, layer_idx, this] () {
                  Entity* map_root = this->get_entity("MapRootEntity");
                  Entity* tile_popup = this->get_entity("TilePopup");
                  Entity* tile_popup_decoration = this->get_entity("TilePopupDecoration");
                  assert(map_root && tile_popup);

                  map_root->get<TileMap>()->remove(*tile_ref);

                  int tile_idx = -1;
                  int num_tiles_in_popup = 0;
                  for (unsigned int i = 0; i < tile_popup->space()->num_children(); ++i) {
                     Entity* e = this->get_entity(tile_popup->space()->get_child(i)->entity());
                     if (e->id().rfind("tile_layer_", 0) == 0) {
                        num_tiles_in_popup++;
                     }

                     if(e->id() == "tile_layer_" + std::to_string(layer_idx)) {
                        tile_idx = i;
                     }
                  }
                  assert(tile_idx != -1);

                  if (num_tiles_in_popup <= 1) {
                     // tile popup only has 1 tile in it, so delete the whole thing
                     this->remove_entity(tile_popup->handle(), true);
                     return;
                  }

                  // shift tiles up by one
                  for (unsigned int l_idx = tile_idx + 1; l_idx < tile_popup->space()->num_children() - 1; ++l_idx) {
                     tile_popup->space()->get_child(l_idx)->move(0, -64);
                  }

                  this->remove_entity(t->handle(), true);

                  // resize tile popup window
                  sf::Vector2f new_popup_size(tile_popup->get<Rectangle>()->size());
                  new_popup_size.y -= 64;

                  tile_popup->get<Rectangle>()->size(new_popup_size);
                  tile_popup->get<Collision>()->volume(tile_popup->get<Rectangle>()->local_bounds());

                  if (tile_popup_decoration) {
                     new_popup_size = tile_popup_decoration->get<Rectangle>()->size();
                     new_popup_size.y -= 64;

                     tile_popup_decoration->get<Rectangle>()->size(new_popup_size);
                  }
               });

               delete_button->space()->move(64 - delete_button->get<Sprite>()->global_bounds().width, 0);

               this->add_to_scene_node(t, delete_button);

               ++layer_idx;
            }

            // we right clicked (not dragged) so highlight this tile with the tile cursor
            pos = grid_entity->get<Grid>()->floor(pos);

            // get rid of tile selection if present
            tile_selection->space()->visible(false);
            tile_selection_maproot->get<Collision>()->volume(sf::FloatRect(0, 0, 0, 0));

            // create a tile popup cursor
            tile_popup_cursor = this->create_entity("TilePopupCursor");
            tile_popup_cursor->space()->position(panel_origin - sf::Vector2f(panel_pos.left, panel_pos.top));

            tile_popup_cursor->add<Rectangle>("TilePopupCursorRectangle", 0, 0, 0, 0);
            tile_popup_cursor->get<Rectangle>()->size(tile_width, tile_height);
            tile_popup_cursor->get<Rectangle>()->color(Color(113, 94, 122, 127));
            tile_popup_cursor->get<Rectangle>()->outline_color(Color(211, 206, 218, 127));
            tile_popup_cursor->get<Rectangle>()->outline_thickness(2.0);

            tile_popup_cursor->add<Collision>("TilePopupCursorCollision", sf::FloatRect(0, 0, 0, 0));
            tile_popup_cursor->get<Collision>()->volume(pos, sf::Vector2f(tile_width, tile_height));

            this->add_to_scene_node(tile_popup, tile_popup_cursor);

            // add little tail to the tile popup pointing to the tile selection
            tile_popup->add<VertexList>("TilePopupTail", sf::TrianglesStrip, 3);

            tile_popup->get<VertexList>()->vertex_color(0, Color(113, 94, 122, 255));
            tile_popup->get<VertexList>()->vertex_color(1, Color(113, 94, 122, 255));
            tile_popup->get<VertexList>()->vertex_color(2, Color(113, 94, 122, 255));

            tile_popup->get<VertexList>()->vertex_position(0, sf::Vector2f(0, 5));
            tile_popup->get<VertexList>()->vertex_position(1, sf::Vector2f(-7, 12));
            tile_popup->get<VertexList>()->vertex_position(2, sf::Vector2f(0, 19));
         }
      }
   });

}

void BuilderScene::mouse_script_add_select_behavior(Game& game, Handle mouse_entity) {
   Entity* mouse_cursor_script = this->get_entity(mouse_entity);
   if (!mouse_cursor_script) {
      Game::logger().msg(this->id(), Logger::WARNING, "(mouse_script_add_select_behavior) Mouse cursor script entity not found.");
      return;
   }

   Entity* mouse_cursor = this->get_entity("MouseCursorEntity");

   // change mouse cursor to pointer
   mouse_cursor->get<Sprite>()->texture(this->textures().get("cursors"));
   mouse_cursor->get<Sprite>()->texture_rect(sf::IntRect(0, 0, 4, 4));
   mouse_cursor->get<Sprite>()->offset(0, 0);

   // turn tile selection white
   Entity* tile_selection = this->get_entity("TileSelectionEntity");
   tile_selection->get<Rectangle>()->color(Color(255, 255, 255, 128));
   tile_selection->get<Rectangle>()->outline_color(Color(255, 255, 255, 192));

   mouse_cursor_script->get<Callback>()->camera_resize([mouse_cursor_script, this] () {
      GraphicalSystem* gs = this->get_system<GraphicalSystem>("GraphicalSystem");

      sf::Vector2f new_size = gs->camera()->size();
      new_size.x *= gs->camera()->viewport().width;
      new_size.y *= gs->camera()->viewport().height;

      // make sure the collision volume fills the whole camera
      mouse_cursor_script->get<Collision>()->volume(sf::Vector2f(0, 0), new_size);
   });

   mouse_cursor_script->get<Callback>()->mouse_move([mouse_cursor_script, this] () {
      Entity* selection_rect = this->get_entity("SelectionRectangleEntity");
      Clickable* clickable = mouse_cursor_script->get<Clickable>();

      sf::Vector2f new_pos;
      new_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      if (clickable && clickable->is_left_clicked()) {
         // update selection rectangle
         sf::Vector2f sr_origin = clickable->left_click_pos();

         sf::Vector2f pos;
         sf::Vector2f end;

         pos.x = std::min(sr_origin.x, new_pos.x);
         pos.y = std::min(sr_origin.y, new_pos.y);
         
         end.x = std::max(sr_origin.x, new_pos.x);
         end.y = std::max(sr_origin.y, new_pos.y);

         selection_rect->space()->position(pos);
         selection_rect->get<Rectangle>()->size(end - pos);
      }
   });

   mouse_cursor_script->get<Callback>()->left_click([&game, mouse_entity, this] () {
      Entity* selection_rect = this->get_entity("SelectionRectangleEntity");
      Entity* tile_selection = this->get_entity("TileSelectionEntity");
      assert(tile_selection);

      sf::Vector2f new_pos;
      new_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // summon selection rectangle
      selection_rect->space()->visible(true);
      selection_rect->space()->position(new_pos);
      selection_rect->get<Rectangle>()->size(0, 0);
   });

   mouse_cursor_script->get<Callback>()->left_release([mouse_cursor_script, this] () {
      Entity* selection_rect = this->get_entity("SelectionRectangleEntity");
      Entity* tile_selection = this->get_entity("TileSelectionEntity");
      Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");
      Entity* grid_root = this->get_entity("GridRootEntity");
      Entity* grid_entity = this->get_entity("GridEntity");

      // release_pos and ts_origin are in screen space
      sf::Vector2f release_pos;
      release_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      release_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      Clickable* clickable = mouse_cursor_script->get<Clickable>();
      assert (clickable);

      sf::Vector2f ts_origin = clickable->left_click_pos();

      // get rid of selection rect
      selection_rect->space()->visible(false);

      // rectangle calculation
      sf::Vector2f pos;
      sf::Vector2f end;

      pos.x = std::min(ts_origin.x, release_pos.x);
      pos.y = std::min(ts_origin.y, release_pos.y);
      
      end.x = std::max(ts_origin.x, release_pos.x);
      end.y = std::max(ts_origin.y, release_pos.y);

      // update tile selection
      bool is_drag_gesture = (end.x - pos.x >= grid_entity->get<Grid>()->tile_width() / 3.f ||
                              end.y - pos.y >= grid_entity->get<Grid>()->tile_height() / 3.f);

      sf::FloatRect tile_selection_global_vol = this->global_transform(*tile_selection).transformRect(tile_selection->get<Collision>()->volume());
      bool clicked_on_tile_selection = tile_selection_global_vol.contains(release_pos);

      if (!is_drag_gesture && tile_selection->space()->visible() && clicked_on_tile_selection) {
         tile_selection->space()->visible(false);
         tile_selection_maproot->get<Collision>()->volume(sf::FloatRect(0, 0, 0, 0));
      } else {
         float tile_width = grid_entity->get<Grid>()->tile_width() * grid_entity->get<Grid>()->zoom_factor().x;
         float tile_height = grid_entity->get<Grid>()->tile_height() * grid_entity->get<Grid>()->zoom_factor().y;

         // get inverse to reverse grid_root translation
         sf::Vector2f world_pos = this->global_transform(*grid_root).getInverse().transformPoint(pos);
         sf::Vector2f world_end = this->global_transform(*grid_root).getInverse().transformPoint(end);

         // round rectangle to nearest grid point
         world_pos = grid_entity->get<Grid>()->floor(world_pos);
         world_end = grid_entity->get<Grid>()->ceil(world_end);

         // make this at least 1 tile big (if you provide grid with value that falls
         // directly on gridline it can give identical values for floor and ceil)
         world_end.x = std::max(world_end.x, world_pos.x + tile_width);
         world_end.y = std::max(world_end.y, world_pos.y + tile_height);

         // update tile selection entity
         tile_selection->space()->position(world_pos);
         tile_selection->space()->visible(true);
         tile_selection->get<Rectangle>()->size(world_end - world_pos);
         tile_selection->get<Collision>()->volume(sf::Vector2f(0, 0), world_end - world_pos);

         // update tile selection maproot entity
         // So the strategy here is that we need to convert pos and end from screen coordinates to map_root coordinates
         // but not including the zoom. Thus using getInverse transform of map_root won't work.
         // We can get the grid indices of the world_pos and world_end and then calculate the
         // corresponding map_root values
         sf::Vector2i map_pos_idx = grid_entity->get<Grid>()->grid_index(world_pos);
         sf::Vector2i map_end_idx = grid_entity->get<Grid>()->grid_index(world_end);

         sf::Vector2f map_pos(
            map_pos_idx.x * grid_entity->get<Grid>()->tile_width(),
            map_pos_idx.y * grid_entity->get<Grid>()->tile_height()
         );

         sf::Vector2f map_end(
            map_end_idx.x * grid_entity->get<Grid>()->tile_width(),
            map_end_idx.y * grid_entity->get<Grid>()->tile_height()
         );

         tile_selection_maproot->space()->position(map_pos);
         tile_selection_maproot->get<Collision>()->volume(sf::Vector2f(0, 0), map_end - map_pos);
      }

      // close tile popup if it exists (do not finish selection)
      Entity* tile_popup = this->get_entity("TilePopup");
      if (tile_popup) {
         this->remove_entity(tile_popup->handle(), true);

         if (!is_drag_gesture) {
            tile_selection->space()->visible(false);
            tile_selection_maproot->get<Collision>()->volume(sf::FloatRect(0, 0, 0, 0));
         }

         return;
      }
   });
}

void BuilderScene::mouse_script_add_move_behavior(Game& game, Handle mouse_entity, bool is_clicked_initial /* = false */) {
   Entity* mouse_cursor_script = this->get_entity(mouse_entity);
   if (!mouse_cursor_script) {
      Game::logger().msg(this->id(), Logger::WARNING, "(mouse_script_add_select_behavior) Mouse cursor script entity not found.");
      return;
   }

   Entity* mouse_cursor = this->get_entity("MouseCursorEntity");

   // change mouse cursor to crosshairs
   mouse_cursor->get<Sprite>()->texture(this->textures().get("cursors"));
   mouse_cursor->get<Sprite>()->texture_rect(sf::IntRect(20, 0, 7, 7));
   mouse_cursor->get<Sprite>()->offset(-10, -10);

   // HACKITY HACK
   std::shared_ptr<bool> is_clicked = std::make_shared<bool>(is_clicked_initial);
   std::shared_ptr<std::vector<Handle>> move_list = std::make_shared<std::vector<Handle>>();

   // turn tile selection blue
   Entity* tile_selection = this->get_entity("TileSelectionEntity");
   tile_selection->get<Rectangle>()->color(Color(96, 157, 255, 128));
   tile_selection->get<Rectangle>()->outline_color(Color(25, 113, 255, 192));

   mouse_cursor_script->get<Callback>()->camera_resize([mouse_cursor_script, this] () {
      GraphicalSystem* gs = this->get_system<GraphicalSystem>("GraphicalSystem");

      sf::Vector2f new_size = gs->camera()->size();
      new_size.x *= gs->camera()->viewport().width;
      new_size.y *= gs->camera()->viewport().height;

      // make sure the collision volume fills the whole camera
      mouse_cursor_script->get<Collision>()->volume(sf::Vector2f(0, 0), new_size);
   });

   mouse_cursor_script->get<Callback>()->mouse_move([mouse_cursor_script, is_clicked, move_list, this] () {
      if (!(*is_clicked)) {
         return;
      }

      Entity* map_root = this->get_entity("MapRootEntity");
      Entity* tile_selection = this->get_entity("TileSelectionEntity");
      Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");

      // calculate mouse move delta
      sf::Vector2f mouse_delta;
      mouse_delta.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      mouse_delta.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      mouse_delta -= mouse_cursor_script->get<Callback>()->prev_mouse_pos();

      // move tile selection cursor
      tile_selection->space()->move(mouse_delta);

      // need to multiply delta by inverse zoom factor for everything under map_root
      mouse_delta.x /= map_root->space()->scale().x;
      mouse_delta.y /= map_root->space()->scale().y;

      tile_selection_maproot->space()->move(mouse_delta);

      if (move_list->empty()) {
         return;
      }

      // move map tiles
      for (Handle handle : *move_list) {
         Entity* e = this->get_entity(handle);
         e->space()->move(mouse_delta);
      }
   });

   mouse_cursor_script->get<Callback>()->left_click([is_clicked, move_list, this] () {
      Entity* map_root = this->get_entity("MapRootEntity");
      Entity* tile_selection = this->get_entity("TileSelectionEntity");
      Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");

      sf::Vector2f new_pos;
      new_pos.x = this->game().get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = this->game().get_player(1).bindings().get<MouseYIntent>()->element()->position();

      sf::FloatRect tile_select_global_vol = this->global_transform(*tile_selection).transformRect(tile_selection->get<Collision>()->volume());
      if (!tile_selection->space()->visible() || !tile_select_global_vol.contains(new_pos)) {
         return;
      }
      
      // don't need global transform because the TileMap doesn't use the global transform
      sf::FloatRect selection_area = tile_selection_maproot->get<Collision>()->volume();
      selection_area.left = tile_selection_maproot->space()->position().x;
      selection_area.top = tile_selection_maproot->space()->position().y;

      // populate move list
      std::vector<TileMap::TileType*> tiles = map_root->get<TileMap>()->find(selection_area);

      if (tiles.size() == 0) {
         return;
      }

      sf::Vector2f min_ts_pos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
      sf::Vector2f max_ts_pos(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

      sf::Vector2f tile_pos;
      sf::FloatRect tile_bounds;

      for (auto tile : tiles) {
         Entity* e = this->create_entity("move_list_" + std::to_string(move_list->size()));
         this->add_to_scene_node(map_root, e);
         move_list->push_back(e->handle());

         // set opacity to 50%
         if (tile->type() == typeid(Circle)) {
            Color c(boost::get<Circle>(*tile).color());
            c.a(192);

            e->add<Circle>(boost::get<Circle>(*tile));
            e->get<Circle>()->color(c);

            tile_pos = boost::get<Circle>(*tile).offset();
            tile_bounds = boost::get<Circle>(*tile).local_bounds();
         } else if (tile->type() == typeid(Rectangle)) {
            Color c(boost::get<Rectangle>(*tile).color());
            c.a(192);

            e->add<Rectangle>(boost::get<Rectangle>(*tile));
            e->get<Rectangle>()->color(c);

            tile_pos = boost::get<Rectangle>(*tile).offset();
            tile_bounds = boost::get<Rectangle>(*tile).local_bounds();
         } else if (tile->type() == typeid(Sprite)) {
            e->add<Sprite>(boost::get<Sprite>(*tile));
            e->get<Sprite>()->color(sf::Color(255, 255, 255, 192));

            tile_pos = boost::get<Sprite>(*tile).offset();
            tile_bounds = boost::get<Sprite>(*tile).local_bounds();
         } else if (tile->type() == typeid(Text)) {
            Color c(boost::get<Text>(*tile).color());
            c.a(192);

            e->add<Text>(boost::get<Text>(*tile));
            e->get<Text>()->color(c);

            tile_pos = boost::get<Text>(*tile).offset();
            tile_bounds = boost::get<Text>(*tile).local_bounds();
         } else if (tile->type() == typeid(VertexList)) {
            Color c(boost::get<VertexList>(*tile).color());
            c.a(192);

            e->add<VertexList>(boost::get<VertexList>(*tile));
            e->get<VertexList>()->color(c);

            tile_pos = boost::get<VertexList>(*tile).offset();
            tile_bounds = boost::get<VertexList>(*tile).local_bounds();
         }

         if (tile_pos.x < min_ts_pos.x) {
            min_ts_pos.x = tile_pos.x;
         }

         if (tile_pos.x + tile_bounds.width > max_ts_pos.x) {
            max_ts_pos.x = tile_pos.x + tile_bounds.width;
         }

         if (tile_pos.y < min_ts_pos.y) {
            min_ts_pos.y = tile_pos.y;
         }

         if (tile_pos.y + tile_bounds.height > max_ts_pos.y) {
            max_ts_pos.y = tile_pos.y + tile_bounds.height;
         }
      }

      map_root->get<TileMap>()->remove(selection_area);

      // resize the tile selection rectangle to fit the selected tiles only
      // we want the corresponding grid_root position of this map_root location
      // so we apply the map_root transform to it (position and zoom) after
      // removing the map root's translational component
      sf::Vector2f corresponding_map_pos = this->global_transform(*map_root).transformPoint(min_ts_pos);
      sf::Vector2f corresponding_map_end = this->global_transform(*map_root).transformPoint(max_ts_pos);

      // remove the translational component
      corresponding_map_pos -= this->global_transform(*map_root).transformPoint(0, 0);
      corresponding_map_end -= this->global_transform(*map_root).transformPoint(0, 0);

      tile_selection->space()->position(corresponding_map_pos);
      tile_selection->get<Rectangle>()->size(corresponding_map_end - corresponding_map_pos);
      tile_selection->get<Collision>()->volume(sf::Vector2f(0, 0), corresponding_map_end - corresponding_map_pos);

      tile_selection_maproot->get<Collision>()->volume(sf::Vector2f(0, 0), max_ts_pos - min_ts_pos);

      *is_clicked = true;
   });

   mouse_cursor_script->get<Callback>()->left_release([&game, mouse_entity, mouse_cursor_script, is_clicked, move_list, this] () {
      Entity* map_root = this->get_entity("MapRootEntity");
      Entity* grid_entity = this->get_entity("GridEntity");
      Entity* tile_selection = this->get_entity("TileSelectionEntity");
      Entity* tile_selection_maproot = this->get_entity("TileSelectionMapRootEntity");

      if (!(*is_clicked)) {
         return;
      }

      *is_clicked = false;

      // snap tile selection and move_list tiles to grid
      tile_selection->space()->position(grid_entity->get<Grid>()->round(tile_selection->space()->position()));
      tile_selection_maproot->space()->position(
         // note: don't use the grid's zoom factor for this round because tile_selection_maproot is on map_root
         grid_entity->get<Grid>()->round(tile_selection_maproot->space()->position(), 1.f)
      );
      
      // add move_list tiles back to TileMap
      for (std::vector<Handle>::const_iterator it = move_list->begin(); it != move_list->end(); ++it) {
         Entity* e = this->get_entity(*it);

         // note: the move behavior mouse delta is stored in e->space()->position(), but
         // each tile's offset is still kept in [component]->offset(), so we need to add
         // them together before rounding to the grid and then overwriting the offset()
         if (e->get<Circle>()) {
            Circle* c = e->get<Circle>();
            c->offset(grid_entity->get<Grid>()->round(e->space()->position() + c->offset(), 1.f));

            Color color(c->color());
            color.a(255);
            c->color(color);

            map_root->get<TileMap>()->set(*c);
         } else if (e->get<Rectangle>()) {
            Rectangle* r = e->get<Rectangle>();
            r->offset(grid_entity->get<Grid>()->round(e->space()->position() + r->offset(), 1.f));

            Color color(r->color());
            color.a(255);
            r->color(color);

            map_root->get<TileMap>()->set(*r);
         } else if (e->get<Sprite>()) {
            Sprite* s = e->get<Sprite>();
            s->offset(grid_entity->get<Grid>()->round(e->space()->position() + s->offset(), 1.f));
            s->color(sf::Color(255, 255, 255, 255));

            map_root->get<TileMap>()->set(*s);
         } else if (e->get<Text>()) {
            Text* t = e->get<Text>();
            t->offset(grid_entity->get<Grid>()->round(e->space()->position() + t->offset(), 1.f));

            Color color(t->color());
            color.a(255);
            t->color(color);

            map_root->get<TileMap>()->set(*t);
         } else if (e->get<VertexList>()) {
            VertexList* v = e->get<VertexList>();
            v->offset(grid_entity->get<Grid>()->round(e->space()->position() + v->offset(), 1.f));

            Color color(v->color());
            color.a(255);
            v->color(color);

            map_root->get<TileMap>()->set(*v);
         }

         this->remove_entity(*it);
      }

      move_list->clear();
   });
}

Handle BuilderScene::create_notification(float width, float height) {
   Entity* hud_root = this->get_entity("HudRootEntity");
   Entity* notification_root = this->create_entity("NotificationRootEntity");

   notification_root->add<Rectangle>("NotificationBackdrop", 0, 0, this->game().window().size().x, this->game().window().size().y);
   notification_root->get<Rectangle>()->color(sf::Color(0, 0, 0, 96));

   notification_root->add<PlayerProfile>("NotificationPlayerProfile", 1);
   notification_root->add<Clickable>("NotificationClickable");
   notification_root->add<Collision>("NotificationCollision", notification_root->get<Rectangle>()->local_bounds());
   notification_root->add<Callback>("NotificationCallback");

   notification_root->get<Callback>()->left_release([this, notification_root] () {
      this->remove_entity(notification_root->handle(), true);
   });

   notification_root->get<Callback>()->right_release([this, notification_root] () {
      this->remove_entity(notification_root->handle(), true);
   });

   notification_root->get<Callback>()->camera_resize([this, notification_root] () {
      notification_root->get<Rectangle>()->size(this->game().window().size());
   });

   this->add_to_scene_node(hud_root, notification_root);

   Entity* notification_box = this->get_entity(this->create_panel("NotificationBox", sf::FloatRect(this->game().window().size().x / 2.f - width / 2.f, this->game().window().size().y / 2.f - height / 2.f, width, height), true));

   notification_box->add<PlayerProfile>("NotificationBoxPlayerProfile", 1);
   notification_box->add<Clickable>("NotificationBoxClickable");
   notification_box->add<Collision>("NotificationBoxCollision", notification_box->get<Rectangle>()->local_bounds());
   notification_box->add<Callback>("NotificationBoxCallback", false);

   notification_box->get<Callback>()->camera_resize([this, notification_box] () {
      sf::Vector2f screen_center(this->game().window().size() / 2.f);
      notification_box->space()->position(screen_center - notification_box->get<Rectangle>()->size() / 2.f);
   });

   this->add_to_scene_node(notification_root, notification_box);

   return notification_box->handle();
}

Handle BuilderScene::create_textbox(
   std::string entity_id,
   float width,
   int charsize /* = 12 */,
   unsigned int max_len /* = 30 */
) {
   int textbox_height = std::ceil(charsize * 1.2f) + 3;
   Color textbox_bg(89, 74, 96, 255);
   Color textbox_outline(196, 191, 203, 230);

   Entity* tb = this->create_entity(entity_id);
   
   tb->add<Rectangle>(entity_id + "Rectangle", 0, 0, width, textbox_height);
   tb->get<Rectangle>()->color(textbox_bg);
   tb->get<Rectangle>()->outline_color(textbox_outline);
   tb->get<Rectangle>()->outline_thickness(2.0);

   tb->add<Text>(entity_id + "Text", "", this->fonts().get("retro"), 12);
   tb->get<Text>()->offset(10, 2);
   tb->get<Text>()->color(textbox_outline);

   tb->add<PlayerProfile>(entity_id + "PlayerProfile", 1);
   tb->add<Clickable>(entity_id + "Clickable");
   tb->add<Collision>(entity_id + "Collision", tb->get<Rectangle>()->local_bounds());
   tb->add<Callback>(entity_id + "Callback");

   tb->get<Callback>()->on_update([this, tb, max_len] () {
      assert(tb->get<Text>());

      InputDevice* keyboard = this->game().input_manager().get_device(1);

      std::string new_char;
      std::string new_value = tb->get<Text>()->string();

      KB_CAPTURE("A", "a", "A");
      KB_CAPTURE("B", "b", "B");
      KB_CAPTURE("C", "c", "C");
      KB_CAPTURE("D", "d", "D");
      KB_CAPTURE("E", "e", "E");
      KB_CAPTURE("F", "f", "F");
      KB_CAPTURE("G", "g", "G");
      KB_CAPTURE("H", "h", "H");
      KB_CAPTURE("I", "i", "I");
      KB_CAPTURE("J", "j", "J");
      KB_CAPTURE("K", "k", "K");
      KB_CAPTURE("L", "l", "L");
      KB_CAPTURE("M", "m", "M");
      KB_CAPTURE("N", "n", "N");
      KB_CAPTURE("O", "o", "O");
      KB_CAPTURE("P", "p", "P");
      KB_CAPTURE("Q", "q", "Q");
      KB_CAPTURE("R", "r", "R");
      KB_CAPTURE("S", "s", "S");
      KB_CAPTURE("T", "t", "T");
      KB_CAPTURE("U", "u", "U");
      KB_CAPTURE("V", "v", "V");
      KB_CAPTURE("W", "w", "W");
      KB_CAPTURE("X", "x", "X");
      KB_CAPTURE("Y", "y", "Y");
      KB_CAPTURE("Z", "z", "Z");
      KB_CAPTURE("Num0", "0", ")");
      KB_CAPTURE("Num1", "1", "!");
      KB_CAPTURE("Num2", "2", "@");
      KB_CAPTURE("Num3", "3", "#");
      KB_CAPTURE("Num4", "4", "$");
      KB_CAPTURE("Num5", "5", "%");
      KB_CAPTURE("Num6", "6", "^");
      KB_CAPTURE("Num7", "7", "&");
      KB_CAPTURE("Num8", "8", "*");
      KB_CAPTURE("Num9", "9", "(");
      KB_CAPTURE("Numpad0", "0", "0");
      KB_CAPTURE("Numpad1", "1", "1");
      KB_CAPTURE("Numpad2", "2", "2");
      KB_CAPTURE("Numpad3", "3", "3");
      KB_CAPTURE("Numpad4", "4", "4");
      KB_CAPTURE("Numpad5", "5", "5");
      KB_CAPTURE("Numpad6", "6", "6");
      KB_CAPTURE("Numpad7", "7", "7");
      KB_CAPTURE("Numpad8", "8", "8");
      KB_CAPTURE("Numpad9", "9", "9");
      KB_CAPTURE("Dash", "-", "_");
      KB_CAPTURE("Equal", "=", "+");
      KB_CAPTURE("Space", " ", " ");
      KB_CAPTURE("SemiColon", ";", ":");
      KB_CAPTURE("Comma", ",", "<");
      KB_CAPTURE("Period", ".", ">");
      KB_CAPTURE("Slash", "/", "?");
      KB_CAPTURE("BackSlash", "\\", "|");

      if (keyboard->get("Backspace")->was_pressed() && new_value.size() > 0) {
         new_value.pop_back();
         tb->get<Text>()->string(new_value);
      }

      if (new_char != "") {
         if (new_value.size() < max_len) {
            new_value += new_char;
         }

         tb->get<Text>()->string(new_value);
      }
   });

   return tb->handle();
}
