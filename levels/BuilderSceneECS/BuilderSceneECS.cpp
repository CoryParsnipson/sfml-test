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

#include "FileChannel.h"
#include "JSONSerializer.h"

#include "MouseXIntent.h"
#include "MouseYIntent.h"
#include "MouseWheelIntent.h"
#include "LeftClickIntent.h"
#include "RightClickIntent.h"

#include "PreorderEntitySubscription.h"

#include "GraphicalSystem.h"
#include "VisualDebugSystem.h"

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

   // TODO: make a callback or system to enable/disable this via user input
   //this->add_system(new VisualDebugSystem("VisualDebugSystemLeft", game.window(), gs->camera()));

   // load or create a tile map
   JSONSerializer serializer;
   FileChannel map_file("tilemap_test.txt");

   map_file.seek(0);
   std::string tilemap_data = serializer.read(map_file);

   map_root->add<TileMap>();
   if (!tilemap_data.empty()) {
      map_root->get<TileMap>()->deserialize(static_cast<Serializer&>(serializer), *this, tilemap_data);
   }

   // create mouse cursor
   Entity* mouse_cursor = this->get_entity(this->create_entity());
   mouse_cursor->id("MouseCursorEntity");

   // add mouse_cursor to the hud_root
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), mouse_cursor->handle());

   mouse_cursor->add<Clickable>("MouseCursorClickable");
   mouse_cursor->add<Collision>("MouseCursorCollision", sf::FloatRect(0, 0, game.window().size().x, game.window().size().y));

   mouse_cursor->add<Rectangle>("MouseCursorRectangle", 0, 0, 6, 6);
   mouse_cursor->get<Rectangle>()->color(sf::Color::Red);

   mouse_cursor->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   // create mouse cursor text
   Entity* mouse_cursor_text = this->get_entity(this->create_entity());
   mouse_cursor_text->id("MouseCursorTextEntity");

   mouse_cursor_text->add<Text>("MouseCursorText", "0, 0", this->fonts().get("retro"), 12);
   mouse_cursor_text->get<Text>()->position(0, 10);
   mouse_cursor_text->get<Text>()->color(sf::Color::White);

   this->send_message_async<AddToEntityMessage>(mouse_cursor->handle(), mouse_cursor_text->handle());

   // create selection rect
   Entity* selection_rect = this->get_entity(this->create_entity());
   selection_rect->id("SelectionRectangleEntity");
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), selection_rect->handle(), 0);

   selection_rect->add<Rectangle>("SelectionRectangle", 0, 0, 0, 0);
   selection_rect->get<Rectangle>()->color(sf::Color(66, 108, 167, 175));
   selection_rect->get<Rectangle>()->outline_color(sf::Color(124, 160, 210, 192));
   selection_rect->get<Rectangle>()->outline_thickness(1.f);

   selection_rect->add<Collision>("SelectionRectandleCollision", sf::FloatRect(0, 0, 0, 0));

   // define mouse cursor behavior
   mouse_cursor->add<Callback>("MouseCursorCallback");
   mouse_cursor->get<Callback>()->mouse_move([mouse_cursor, mouse_cursor_text, selection_rect, gs, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // move mouse cursor
      mouse_cursor->get<Rectangle>()->position(new_pos - sf::Vector2f(3, 3));

      // move and update mouse cursor text
      mouse_cursor_text->get<Text>()->position(new_pos + sf::Vector2f(0, 10));
      mouse_cursor_text->get<Text>()->string(
         std::to_string(static_cast<int>(new_pos.x)) +
         ", " +
         std::to_string(static_cast<int>(new_pos.y))
      );

      // update selection rectangle
      Clickable* clickable = mouse_cursor->get<Clickable>();
      if (clickable && clickable->is_left_clicked()) {
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
      Callback* callback = mouse_cursor->get<Callback>();
      if (clickable && callback && clickable->is_right_clicked()) {
         gs->camera()->move(callback->prev_mouse_pos() - new_pos);
      }
   });

   mouse_cursor->get<Callback>()->mouse_wheel([&game] () {
      float wheel_delta = game.get_player(1).bindings().get<MouseWheelIntent>()->element()->position();
      Game::logger().msg("asdf", Logger::INFO, "Mouse delta " + std::to_string(wheel_delta));
   });

   mouse_cursor->get<Callback>()->left_click([selection_rect, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      // set selection rect origin
      selection_rect->get<Rectangle>()->position(new_pos);
   });

   mouse_cursor->get<Callback>()->left_release([selection_rect] () {
      selection_rect->get<Rectangle>()->size(0, 0);
      selection_rect->get<Collision>()->volume(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
   });

   mouse_cursor->get<Callback>()->camera_resize([mouse_cursor, gs] () {
      sf::Vector2f new_size = gs->camera()->get_size();
      new_size.x *= gs->camera()->get_viewport().width;
      new_size.y *= gs->camera()->get_viewport().height;

      // make sure the mouse cursor collision volume fills the whole camera
      mouse_cursor->get<Collision>()->volume(sf::Vector2f(0, 0), new_size);
   });
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
