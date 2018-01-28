#include "sfml.h"

#include "BuilderSceneECS.h"

#include "Game.h"
#include "Entity.h"

#include "Text.h"
#include "Rectangle.h"
#include "Callback.h"
#include "Clickable.h"
#include "Collision.h"
#include "PlayerProfile.h"
#include "TileMap.h"
#include "Grid2.h"

#include "FileChannel.h"
#include "JSONSerializer.h"

#include "MouseXIntent.h"
#include "MouseYIntent.h"
#include "MouseWheelIntent.h"
#include "LeftClickIntent.h"
#include "RightClickIntent.h"
#include "VisualDebugIntent.h"
#include "GridVisibilityToggleIntent.h"
#include "ResetCameraIntent.h"
#include "RemoveTilesIntent.h"
#include "MoveUpIntent.h"
#include "MoveLeftIntent.h"
#include "MoveRightIntent.h"
#include "MoveDownIntent.h"

#include "PreorderEntitySubscription.h"

#include "GraphicalSystem.h"
#include "VisualDebugSystem.h"
#include "GridSystem.h"
#include "BuilderSceneECSInputSystem.h"

#include "AddToEntityMessage.h"

BuilderSceneECS::BuilderSceneECS()
: Scene("BuilderSceneECS")
{
}

BuilderSceneECS::~BuilderSceneECS() {
}

void BuilderSceneECS::init(Game& game) {
   // load fonts
   this->fonts().load("retro", "retro.ttf");
   
   // load textures
   this->textures().load("tile_grass", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 0, 64, 64));
   this->textures().load("tile_worn_grass", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 0, 64, 64));
   this->textures().load("tile_sign", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 0, 64, 64));
   this->textures().load("tile_dirt_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 64, 64, 64));
   this->textures().load("tile_dirt_um", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 64, 64, 64));
   this->textures().load("tile_dirt_ur", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 64, 64, 64));
   this->textures().load("tile_dirt_ml", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 128, 64, 64));
   this->textures().load("tile_dirt_mm", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 128, 64, 64));
   this->textures().load("tile_dirt_mr", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 128, 64, 64));
   this->textures().load("tile_dirt_bl", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 192, 64, 64));
   this->textures().load("tile_dirt_bm", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 192, 64, 64));
   this->textures().load("tile_dirt_br", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 192, 64, 64));
   this->textures().load("tile_water_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 64, 64, 64));
   this->textures().load("tile_water_um", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 64, 64, 64));
   this->textures().load("tile_water_ur", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 64, 64, 64));
   this->textures().load("tile_water_ml", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 128, 64, 64));
   this->textures().load("tile_water_mm", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 128, 64, 64));
   this->textures().load("tile_water_mr", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 128, 64, 64));
   this->textures().load("tile_water_bl", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 192, 64, 64));
   this->textures().load("tile_water_bm", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 192, 64, 64));
   this->textures().load("tile_water_br", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 192, 64, 64));
   Game::logger().msg(this->id(), Logger::INFO, this->textures());

   // setup player's input mapping
   game.add_player(1);
   game.get_player(1).bindings().set<MouseXIntent>(0, game.input_manager().get_device(0)->get("PositionX"));
   game.get_player(1).bindings().set<MouseYIntent>(0, game.input_manager().get_device(0)->get("PositionY"));
   game.get_player(1).bindings().set<MouseWheelIntent>(0, game.input_manager().get_device(0)->get("Wheel"));
   game.get_player(1).bindings().set<LeftClickIntent>(0, game.input_manager().get_device(0)->get("Left"));
   game.get_player(1).bindings().set<RightClickIntent>(0, game.input_manager().get_device(0)->get("Right"));

   game.get_player(1).bindings().set<GridVisibilityToggleIntent>(1, game.input_manager().get_device(1)->get("G"));
   game.get_player(1).bindings().set<VisualDebugIntent>(1, game.input_manager().get_device(1)->get("D"));
   game.get_player(1).bindings().set<ResetCameraIntent>(1, game.input_manager().get_device(1)->get("R"));
   game.get_player(1).bindings().set<RemoveTilesIntent>(1, game.input_manager().get_device(1)->get("Delete"));

   game.get_player(1).bindings().set<MoveUpIntent>(1, game.input_manager().get_device(1)->get("Up"));
   game.get_player(1).bindings().set<MoveLeftIntent>(1, game.input_manager().get_device(1)->get("Left"));
   game.get_player(1).bindings().set<MoveRightIntent>(1, game.input_manager().get_device(1)->get("Right"));
   game.get_player(1).bindings().set<MoveDownIntent>(1, game.input_manager().get_device(1)->get("Down"));

   // make a map root entity and hud root entity
   Entity* map_root = this->get_entity(this->create_entity());
   Entity* hud_root = this->get_entity(this->create_entity());

   map_root->id("MapRoot Entity");
   hud_root->id("HudRoot Entity");

   // TODO: create a better way to get Systems
   GraphicalSystem* gs = dynamic_cast<GraphicalSystem*>(this->get_system("GraphicalSystem"));
   gs->root(map_root->handle());

   GraphicalSystem* hud_graphics = new GraphicalSystem("HudGraphics", game.window(), std::make_shared<Camera>("HudCamera"));
   hud_graphics->root(hud_root->handle());
   this->add_system(hud_graphics);

   VisualDebugSystem* map_vds = new VisualDebugSystem("MapVisualDebugSystem", game.window(), gs->camera());
   map_vds->disable(); // start with this off
   map_vds->root(map_root->handle());
   this->add_system(map_vds);

   VisualDebugSystem* hud_vds = new VisualDebugSystem("HudVisualDebugSystem", game.window(), hud_graphics->camera());
   hud_vds->disable(); // start with this off
   hud_vds->root(hud_root->handle());
   this->add_system(hud_vds);

   // load or create a tile map
   JSONSerializer serializer;
   FileChannel map_file("tilemap_test.txt");

   map_file.seek(0);
   std::string tilemap_data = serializer.read(map_file);

   map_root->add<TileMap>();
   if (!tilemap_data.empty()) {
      map_root->get<TileMap>()->deserialize(static_cast<Serializer&>(serializer), *this, tilemap_data);
   }

   // load or create a grid
   Entity* grid_root = this->get_entity(this->create_entity());
   grid_root->id("GridRoot Entity");
   grid_root->add<Grid2>("GridRoot Grid Component", sf::Vector2f(0, 0), 64, 64);

   this->send_message_async<AddToEntityMessage>(map_root->handle(), grid_root->handle());

   // add grid system
   this->add_system(new GridSystem("GridSystem", gs->camera()));

   // create selection rect
   Entity* selection_rect = this->get_entity(this->create_entity());
   selection_rect->id("SelectionRectangleEntity");
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), selection_rect->handle(), 0);

   selection_rect->add<Rectangle>("SelectionRectangle", 0, 0, 0, 0);
   selection_rect->get<Rectangle>()->color(sf::Color(66, 108, 167, 175));
   selection_rect->get<Rectangle>()->outline_color(sf::Color(124, 160, 210, 192));
   selection_rect->get<Rectangle>()->outline_thickness(1.f);

   selection_rect->add<Collision>("SelectionRectandleCollision", sf::FloatRect(0, 0, 0, 0));

   // create tile selection
   Entity* tile_selection = this->get_entity(this->create_entity());
   tile_selection->id("TileSelectionEntity");
   this->send_message_async<AddToEntityMessage>(map_root->handle(), tile_selection->handle(), 0);

   tile_selection->add<Rectangle>("TileSelectionRectangle", 0, 0, 0, 0);
   tile_selection->get<Rectangle>()->color(sf::Color(255, 255, 255, 128));
   tile_selection->get<Rectangle>()->outline_color(sf::Color(255, 255, 255, 192));
   tile_selection->get<Rectangle>()->outline_thickness(2.0);

   tile_selection->add<Collision>("TileSelectionCollider", sf::FloatRect(0, 0, 0, 0));

   // create tile palette window
   Entity* tile_palette_window = this->get_entity(this->create_entity());
   tile_palette_window->id("TilePaletteWindow");
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), tile_palette_window->handle()); // put this on top of mouse cursor and selection_rect

   tile_palette_window->add<Rectangle>("TilePaletteWindowRectangle", game.window().size().x - 210, 10, 200, 450);
   tile_palette_window->get<Rectangle>()->color(sf::Color(113, 94, 122, 230));

   tile_palette_window->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   tile_palette_window->add<Callback>("TilePaletteWindowCallback", false);
   tile_palette_window->add<Clickable>("TilePaletteWindowClickable");
   tile_palette_window->add<Collision>("TilePaletteWindowCollision", tile_palette_window->get<Rectangle>()->global_bounds());

   Entity* tpw_outline = this->get_entity(this->create_entity());
   tpw_outline->id("TilePaletteWindowDecoration");
   this->send_message_async<AddToEntityMessage>(tile_palette_window->handle(), tpw_outline->handle());

   tpw_outline->add<Rectangle>("", game.window().size().x - 205, 20, 190, 435);
   tpw_outline->get<Rectangle>()->color(sf::Color::Transparent);
   tpw_outline->get<Rectangle>()->outline_color(sf::Color(211, 206, 218, 230));
   tpw_outline->get<Rectangle>()->outline_thickness(2.0);

   // create mouse cursor
   Entity* mouse_cursor = this->get_entity(this->create_entity());
   mouse_cursor->id("MouseCursorEntity");

   // add mouse_cursor to the hud_root
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), mouse_cursor->handle()); // added this last to put it on top

   mouse_cursor->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   mouse_cursor->add<Rectangle>("MouseCursorRectangle", 0, 0, 6, 6);
   mouse_cursor->get<Rectangle>()->color(sf::Color::Red);

   mouse_cursor->add<Text>("MouseCursorText", "0, 0", this->fonts().get("retro"), 12);
   mouse_cursor->get<Text>()->position(0, 10);
   mouse_cursor->get<Text>()->color(sf::Color::White);

   mouse_cursor->add<Callback>("MouseCursorCallback");
   mouse_cursor->get<Callback>()->mouse_move([mouse_cursor, gs, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      sf::Vector2f screen_pos = gs->camera()->get_world_coordinate(new_pos);

      // move mouse cursor
      mouse_cursor->get<Rectangle>()->position(new_pos - sf::Vector2f(3, 3));

      // move and update mouse cursor text
      mouse_cursor->get<Text>()->position(new_pos + sf::Vector2f(0, 10));
      mouse_cursor->get<Text>()->string(
         std::to_string(static_cast<int>(new_pos.x)) +
         ", " +
         std::to_string(static_cast<int>(new_pos.y)) + "\n" +
         std::to_string(static_cast<int>(screen_pos.x)) +
         ", " +
         std::to_string(static_cast<int>(screen_pos.y))
      );
   });

   // This is an invisibile entity that sits at the bottom of the hud layer to handle mouse behavior
   Entity* mouse_cursor_script = this->get_entity(this->create_entity());
   mouse_cursor_script->id("MouseCursorScriptEntity");

   this->send_message_async<AddToEntityMessage>(hud_root->handle(), mouse_cursor_script->handle(), 0);

   mouse_cursor_script->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   mouse_cursor_script->add<Clickable>("MouseCursorClickable");
   mouse_cursor_script->add<Collision>("MouseCursorCollision", sf::FloatRect(0, 0, game.window().size().x, game.window().size().y));

   // define pan and selection tool behavior
   mouse_cursor_script->add<Callback>("MouseCursorScriptCallback");
   mouse_cursor_script->get<Callback>()->mouse_move([mouse_cursor_script, selection_rect, gs, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      Clickable* clickable = mouse_cursor_script->get<Clickable>();
      if (clickable && clickable->is_left_clicked()) {
         // update selection rectangle
         sf::Vector2f sr_origin = clickable->left_click_pos();

         sf::Vector2f pos;
         sf::Vector2f end;

         pos.x = std::min(sr_origin.x, new_pos.x);
         pos.y = std::min(sr_origin.y, new_pos.y);
         
         end.x = std::max(sr_origin.x, new_pos.x);
         end.y = std::max(sr_origin.y, new_pos.y);

         selection_rect->get<Rectangle>()->position(pos);
         selection_rect->get<Rectangle>()->size(end - pos);

         selection_rect->get<Collision>()->volume(pos, end - pos);
      }

      // pan map camera if right click is down
      Callback* callback = mouse_cursor_script->get<Callback>();
      if (clickable && callback && clickable->is_right_clicked()) {
         gs->camera()->move(callback->prev_mouse_pos() - new_pos);
      }
   });

   mouse_cursor_script->get<Callback>()->mouse_wheel([this, gs, &game] () {
      float mouse_wheel_pos = game.get_player(1).bindings().get<MouseWheelIntent>()->element()->position();
      float wheel_delta = mouse_wheel_pos - this->prev_mouse_wheel_pos_;

      gs->camera()->set_scale(gs->camera()->get_scale() + wheel_delta / 15.f);

      this->prev_mouse_wheel_pos_ = mouse_wheel_pos;
   });

   mouse_cursor_script->get<Callback>()->left_click([selection_rect, gs, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // summon selection rectangle
      selection_rect->get<Space>()->visible(true);
      selection_rect->get<Rectangle>()->position(new_pos);
      selection_rect->get<Rectangle>()->size(0, 0);
      selection_rect->get<Collision>()->volume(new_pos, sf::Vector2f(0, 0));
   });

   mouse_cursor_script->get<Callback>()->left_release([selection_rect, tile_selection, mouse_cursor_script, grid_root, gs, &game] () {
      sf::Vector2f release_pos;
      release_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      release_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();
      release_pos = gs->camera()->get_world_coordinate(release_pos);

      Clickable* clickable = mouse_cursor_script->get<Clickable>();
      assert (clickable);

      sf::Vector2f ts_origin  = gs->camera()->get_world_coordinate(clickable->left_click_pos());

      // rectangle calculation
      sf::Vector2f pos;
      sf::Vector2f end;

      pos.x = std::min(ts_origin.x, release_pos.x);
      pos.y = std::min(ts_origin.y, release_pos.y);
      
      end.x = std::max(ts_origin.x, release_pos.x);
      end.y = std::max(ts_origin.y, release_pos.y);

      sf::FloatRect ts_rect(pos, end - pos);

      // get rid of selection rect
      selection_rect->get<Space>()->visible(false);

      // update tile selection
      bool is_drag_gesture = (ts_rect.width >= grid_root->get<Grid2>()->tile_width() / 3.f ||
                              ts_rect.height >= grid_root->get<Grid2>()->tile_height() / 3.f);

      if (!is_drag_gesture && tile_selection->get<Space>()->visible() && tile_selection->get<Collision>()->contains(release_pos)) {
         tile_selection->get<Space>()->visible(false);
      } else {
         // round rectangle to nearest grid point
         pos = grid_root->get<Grid2>()->floor(pos);
         end = grid_root->get<Grid2>()->ceil(end);
         
         // update tile selection entity
         tile_selection->get<Rectangle>()->position(pos);
         tile_selection->get<Rectangle>()->size(end - pos);
         tile_selection->get<Collision>()->volume(pos, end - pos);
         tile_selection->get<Space>()->visible(true);
      }
   });

   mouse_cursor_script->get<Callback>()->camera_resize([mouse_cursor_script, gs] () {
      sf::Vector2f new_size = gs->camera()->get_size();
      new_size.x *= gs->camera()->get_viewport().width;
      new_size.y *= gs->camera()->get_viewport().height;

      // make sure the mouse cursor script collision volume fills the whole camera
      mouse_cursor_script->get<Collision>()->volume(sf::Vector2f(0, 0), new_size);
   });

   // add keyboard input system
   BuilderSceneECSInputSystem* input_system = new BuilderSceneECSInputSystem();
   input_system->grid_entity = grid_root->handle();
   input_system->map_entity = map_root->handle();
   input_system->tile_selection = tile_selection->handle();
   input_system->map_camera = gs->camera();
   this->add_system(input_system);
}

void BuilderSceneECS::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering builder state.");
}

void BuilderSceneECS::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting builder state.");
}

void BuilderSceneECS::process(Game& game, CloseInputEvent& e) {
   game.unload_scene();
}
