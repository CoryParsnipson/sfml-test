#include <SFML/Graphics.hpp>

#include "BuilderScene.h"

#include "Game.h"
#include "Entity.h"

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

#include "PreorderEntitySubscription.h"

#include "GraphicalSystem.h"
#include "VisualDebugSystem.h"
#include "GridSystem.h"
#include "BuilderSceneInputSystem.h"

#include "AddToEntityMessage.h"

BuilderScene::BuilderScene()
: Scene("BuilderScene")
{
}

BuilderScene::~BuilderScene() {
}

void BuilderScene::init(Game& game) {
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
   game.get_player(1).bindings().set<ResetViewIntent>(1, game.input_manager().get_device(1)->get("R"));
   game.get_player(1).bindings().set<RemoveTilesIntent>(1, game.input_manager().get_device(1)->get("Delete"));
   game.get_player(1).bindings().set<SerializeMapIntent>(1, game.input_manager().get_device(1)->get("S"));

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

   VisualDebugSystem* vds = new VisualDebugSystem("VisualDebugSystem", game.window(), gs->camera());
   vds->disable(); // start with this off
   this->add_system(vds);

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
   grid_root->add<Grid>("GridRoot Grid Component", sf::Vector2f(0, 0), 64, 64);

   this->send_message_async<AddToEntityMessage>(map_root->handle(), grid_root->handle());

   // add grid system
   this->add_system(new GridSystem("GridSystem", gs->camera()));

   // add a nice backdrop
   Entity* backdrop = this->get_entity(this->create_entity());
   backdrop->id("Backdrop Entity");
   this->send_message_async<AddToEntityMessage>(this->space_handle(), backdrop->handle(), 0);
   
   backdrop->add<VertexList>("BackdropVertexList", sf::TrianglesStrip, 4);
   backdrop->get<VertexList>()->vertex_color(0, sf::Color(50, 50, 50, 255));
   backdrop->get<VertexList>()->vertex_color(1, sf::Color(25, 25, 25, 255));
   backdrop->get<VertexList>()->vertex_color(2, sf::Color(50, 50, 50, 255));
   backdrop->get<VertexList>()->vertex_color(3, sf::Color(25, 25, 25, 255));

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

   // create fps display entity
   Entity* fps_display = this->get_entity(this->create_entity());
   fps_display->id("FPS Display Entity");
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), fps_display->handle());

   fps_display->add<Text>("FPS Display Text", "FPS: ??", this->fonts().get("retro"), 12);
   fps_display->get<Text>()->color(sf::Color(216, 138, 0, 255));

   fps_display->add<PlayerProfile>("FPSDisplayPlayerProfile", 1);
   fps_display->add<Callback>("FPSDisplayCallback");

   fps_display->get<Callback>()->camera_resize([fps_display, gs] () {
      sf::Vector2f camera_bounds = gs->camera()->size();
      sf::Vector2f text_size;

      text_size.x = fps_display->get<Text>()->local_bounds().width;
      text_size.y = fps_display->get<Text>()->local_bounds().height;

      camera_bounds.x *= gs->camera()->viewport().width / gs->camera()->zoom();
      camera_bounds.y *= gs->camera()->viewport().height / gs->camera()->zoom();

      fps_display->get<Text>()->position(camera_bounds - text_size - sf::Vector2f(10, 10));
   });

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

   tile_palette_window->add<Rectangle>("TilePaletteWindowRectangle", 0, 0, 220, 480);
   tile_palette_window->get<Rectangle>()->color(sf::Color(113, 94, 122, 255));

   tile_palette_window->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   tile_palette_window->add<Callback>("TilePaletteWindowCallback", false);
   tile_palette_window->add<Clickable>("TilePaletteWindowClickable");
   tile_palette_window->add<Collision>("TilePaletteWindowCollision", tile_palette_window->get<Rectangle>()->global_bounds());

   // readjust tile palette window (and children) when the camera is resized
   tile_palette_window->get<Callback>()->camera_resize([tile_palette_window, gs] () {
      float camera_width = gs->camera()->size().x * gs->camera()->viewport().width / gs->camera()->zoom();
      sf::Vector2f new_pos(camera_width - tile_palette_window->get<Rectangle>()->size().x - 10, 10);
      sf::Vector2f old_pos(tile_palette_window->get<Space>()->position());

      // move to the upper right corner
      tile_palette_window->get<Space>()->move(new_pos - old_pos);
   });

   Entity* tpw_outline = this->get_entity(this->create_entity());
   tpw_outline->id("TilePaletteWindowDecoration");
   this->send_message_async<AddToEntityMessage>(tile_palette_window->handle(), tpw_outline->handle());

   tpw_outline->add<Rectangle>("TilePaletteWindowOutline", 5, 10, tile_palette_window->get<Rectangle>()->size().x - 10, tile_palette_window->get<Rectangle>()->size().y - 15);
   tpw_outline->get<Rectangle>()->color(sf::Color::Transparent);
   tpw_outline->get<Rectangle>()->outline_color(sf::Color(211, 206, 218, 230));
   tpw_outline->get<Rectangle>()->outline_thickness(2.0);

   Entity* tpw_title_back = this->get_entity(this->create_entity());
   tpw_title_back->id("TilePaletteWindowTitleBack");
   this->send_message_async<AddToEntityMessage>(tile_palette_window->handle(), tpw_title_back->handle());

   tpw_title_back->add<Rectangle>("TilePaletteWindowTitleBack", 0, 0, 0, 0);
   tpw_title_back->get<Rectangle>()->color(sf::Color(113, 94, 122, 255));

   Entity* tpw_title = this->get_entity(this->create_entity());
   tpw_title->id("TilePaletteWindowTitle");
   this->send_message_async<AddToEntityMessage>(tile_palette_window->handle(), tpw_title->handle());

   tpw_title->add<Text>("TilePaletteWindowTitleText", "Tileset:", this->fonts().get("retro"), 12);
   tpw_title->get<Text>()->position(30, 2);

   tpw_title_back->get<Rectangle>()->position(tpw_title->get<Text>()->position() - sf::Vector2f(3, 0));
   tpw_title_back->get<Rectangle>()->size(tpw_title->get<Text>()->local_bounds().width + 6, tpw_title->get<Text>()->local_bounds().height);

   Entity* tpw_hover = this->get_entity(this->create_entity());
   tpw_hover->id("TilePaletteWindowHover");
   
   tpw_hover->get<Space>()->visible(false);
   
   tpw_hover->add<Rectangle>("TilePaletteWindowRectangle", 0, 0, grid_root->get<Grid>()->tile_width(), grid_root->get<Grid>()->tile_height());
   tpw_hover->get<Rectangle>()->color(sf::Color(255, 255, 255, 100));
   tpw_hover->get<Rectangle>()->outline_color(sf::Color(108, 46, 167, 100));
   tpw_hover->get<Rectangle>()->outline_thickness(2.f);

   // add tile textures to the tile palette window
   std::vector<std::string> tile_textures;
   tile_textures.push_back("tile_grass");
   tile_textures.push_back("tile_worn_grass");
   tile_textures.push_back("tile_sign");
   tile_textures.push_back("tile_dirt_ul");
   tile_textures.push_back("tile_dirt_um");
   tile_textures.push_back("tile_dirt_ur");
   tile_textures.push_back("tile_dirt_ml");
   tile_textures.push_back("tile_dirt_mm");
   tile_textures.push_back("tile_dirt_mr");
   tile_textures.push_back("tile_dirt_bl");
   tile_textures.push_back("tile_dirt_bm");
   tile_textures.push_back("tile_dirt_br");
   tile_textures.push_back("tile_water_ul");
   tile_textures.push_back("tile_water_um");
   tile_textures.push_back("tile_water_ur");
   tile_textures.push_back("tile_water_ml");
   tile_textures.push_back("tile_water_mm");
   tile_textures.push_back("tile_water_mr");
   tile_textures.push_back("tile_water_bl");
   tile_textures.push_back("tile_water_bm");
   tile_textures.push_back("tile_water_br");

   int tiles_per_row = tile_palette_window->get<Rectangle>()->size().x / grid_root->get<Grid>()->tile_width();
   int side_padding = (tile_palette_window->get<Rectangle>()->size().x - (grid_root->get<Grid>()->tile_width() * tiles_per_row)) / 2;
   int top_padding = 20;

   int i = 0;
   for (std::vector<std::string>::const_iterator it = tile_textures.begin(); it != tile_textures.end(); ++it, ++i) {
      sf::Vector2f tile_texture_pos(
         (i % tiles_per_row) * grid_root->get<Grid>()->tile_width() + side_padding,
         (i / tiles_per_row) * grid_root->get<Grid>()->tile_height() + top_padding
      );

      Entity* entity = this->get_entity(this->create_entity());
      entity->id(*it + "_tpw_entity");
      this->send_message_async<AddToEntityMessage>(tile_palette_window->handle(), entity->handle());

      entity->add<Sprite>(*it + "_sprite", *this->textures().get(*it));
      entity->get<Sprite>()->position(tile_texture_pos);

      entity->add<PlayerProfile>(*it + "_playerProfile", 1);
      entity->add<Clickable>(*it + "_clickable");
      entity->add<Collision>(*it + "_collision", entity->get<Sprite>()->global_bounds());

      // define texture button behavior
      entity->add<Callback>(*it + "_callback");
      entity->get<Callback>()->mouse_in([entity, tpw_hover] () {
         tpw_hover->get<Rectangle>()->position(entity->get<Sprite>()->position());
         tpw_hover->get<Space>()->visible(true);
      });

      entity->get<Callback>()->mouse_out([entity, tpw_hover] () {
         tpw_hover->get<Space>()->visible(false);
      });

      entity->get<Callback>()->left_release([entity, tile_selection, map_root, grid_root] () {
         // remove any existing tiles in the selection if they exist
         map_root->get<TileMap>()->remove(tile_selection->get<Collision>()->volume());

         // on click, fill the selected area (if it is active) with the clicked on tile
         sf::FloatRect tsc = tile_selection->get<Collision>()->volume();
         for (int x_pos = tsc.left; x_pos < tsc.left + tsc.width; x_pos += grid_root->get<Grid>()->tile_width()) {
            for (int y_pos = tsc.top; y_pos < tsc.top + tsc.height; y_pos += grid_root->get<Grid>()->tile_height()) {
               Sprite sprite(*entity->get<Sprite>());
               sprite.position(x_pos, y_pos);

               map_root->get<TileMap>()->set(sprite);
            }
         }
      });
   }

   // putting this here because tpw_hover needs to be on top of all tiles
   this->send_message_async<AddToEntityMessage>(tile_palette_window->handle(), tpw_hover->handle());

   // create mouse cursor
   Entity* mouse_cursor = this->get_entity(this->create_entity());
   mouse_cursor->id("MouseCursorEntity");

   this->mouse_cursor_ = mouse_cursor->handle();

   // add mouse_cursor to the hud_root
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), mouse_cursor->handle()); // added this last to put it on top

   mouse_cursor->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   mouse_cursor->add<Rectangle>("MouseCursorRectangle", 0, 0, 6, 6);
   mouse_cursor->get<Rectangle>()->color(sf::Color::Red);

   mouse_cursor->add<Text>("MouseCursorText", "0, 0", this->fonts().get("retro"), 12);
   mouse_cursor->get<Text>()->position(0, 10);
   mouse_cursor->get<Text>()->color(sf::Color::White);

   mouse_cursor->add<Callback>("MouseCursorCallback");
   mouse_cursor->get<Callback>()->mouse_move([mouse_cursor, map_root, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      sf::Vector2f screen_pos = map_root->get<Space>()->inverse_transform().transformPoint(new_pos);

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

   // This is an invisible entity that sits at the bottom of the hud layer to handle mouse behavior
   Entity* mouse_cursor_script = this->get_entity(this->create_entity());
   mouse_cursor_script->id("MouseCursorScriptEntity");

   this->send_message_async<AddToEntityMessage>(hud_root->handle(), mouse_cursor_script->handle(), 0);

   mouse_cursor_script->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   mouse_cursor_script->add<Clickable>("MouseCursorClickable");
   mouse_cursor_script->add<Collision>("MouseCursorCollision", sf::FloatRect(0, 0, game.window().size().x, game.window().size().y));

   // define pan and selection tool behavior
   mouse_cursor_script->add<Callback>("MouseCursorScriptCallback");
   mouse_cursor_script->get<Callback>()->mouse_move([mouse_cursor_script, selection_rect, map_root, &game] () {
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
         map_root->get<Space>()->move(new_pos - callback->prev_mouse_pos());
      }
   });

   mouse_cursor_script->get<Callback>()->mouse_wheel([mouse_cursor_script, map_root, grid_root, &game] () {
      float mouse_wheel_pos = game.get_player(1).bindings().get<MouseWheelIntent>()->element()->position();
      float wheel_delta = mouse_wheel_pos - mouse_cursor_script->get<Callback>()->prev_mouse_wheel_pos();

      map_root->get<Space>()->scale(map_root->get<Space>()->scale() + sf::Vector2f(wheel_delta / 15.f, wheel_delta / 15.f));
      grid_root->get<Grid>()->zoom_factor = map_root->get<Space>()->scale() + sf::Vector2f(wheel_delta / 15.f, wheel_delta / 15.f);
   });

   mouse_cursor_script->get<Callback>()->left_click([selection_rect, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // summon selection rectangle
      selection_rect->get<Space>()->visible(true);
      selection_rect->get<Rectangle>()->position(new_pos);
      selection_rect->get<Rectangle>()->size(0, 0);
      selection_rect->get<Collision>()->volume(new_pos, sf::Vector2f(0, 0));
   });

   mouse_cursor_script->get<Callback>()->left_release([selection_rect, tile_selection, mouse_cursor_script, grid_root, map_root, &game] () {
      sf::Vector2f release_pos;
      release_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      release_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();
      release_pos = map_root->get<Space>()->inverse_transform().transformPoint(release_pos);

      Clickable* clickable = mouse_cursor_script->get<Clickable>();
      assert (clickable);

      sf::Vector2f ts_origin  = map_root->get<Space>()->inverse_transform().transformPoint(clickable->left_click_pos());

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
      bool is_drag_gesture = (ts_rect.width >= grid_root->get<Grid>()->tile_width() / 3.f ||
                              ts_rect.height >= grid_root->get<Grid>()->tile_height() / 3.f);

      if (!is_drag_gesture && tile_selection->get<Space>()->visible() && tile_selection->get<Collision>()->contains(release_pos)) {
         tile_selection->get<Space>()->visible(false);
      } else {
         // round rectangle to nearest grid point
         pos = grid_root->get<Grid>()->floor(pos);
         end = grid_root->get<Grid>()->ceil(end);
         
         // update tile selection entity
         tile_selection->get<Rectangle>()->position(pos);
         tile_selection->get<Rectangle>()->size(end - pos);
         tile_selection->get<Collision>()->volume(pos, end - pos);
         tile_selection->get<Space>()->visible(true);
      }
   });

   mouse_cursor_script->get<Callback>()->camera_resize([mouse_cursor_script, gs] () {
      sf::Vector2f new_size = gs->camera()->size();
      new_size.x *= gs->camera()->viewport().width;
      new_size.y *= gs->camera()->viewport().height;

      // make sure the mouse cursor script collision volume fills the whole camera
      mouse_cursor_script->get<Collision>()->volume(sf::Vector2f(0, 0), new_size);
   });

   // add keyboard input system
   BuilderSceneInputSystem* input_system = new BuilderSceneInputSystem();
   input_system->grid_entity = grid_root->handle();
   input_system->map_entity = map_root->handle();
   input_system->tile_selection = tile_selection->handle();
   input_system->fps_entity = fps_display->handle();
   input_system->serializer = new JSONSerializer(3);
   input_system->file_channel = new FileChannel("tilemap_test.txt");
   this->add_system(input_system);
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
   Entity* cursor = this->get_entity(this->mouse_cursor_);
   if (cursor) {
      cursor->get<Space>()->visible(true);
   }
}

void BuilderScene::process(Game& game, MouseLeftInputEvent& e) {
   Entity* cursor = this->get_entity(this->mouse_cursor_);
   if (cursor) {
      cursor->get<Space>()->visible(false);
   }
}
