#include "sfml.h"

#include "BuilderSceneECS.h"

#include "Game.h"
#include "Text.h"
#include "Rectangle.h"
#include "Callback.h"
#include "Clickable.h"
#include "Collision.h"
#include "Entity.h"
#include "PlayerProfile.h"

#include "FileChannel.h"

#include "MouseXIntent.h"
#include "MouseYIntent.h"
#include "MouseWheelIntent.h"
#include "LeftClickIntent.h"
#include "RightClickIntent.h"

#include "SpatialEntitySubscription.h"

#include "GraphicalSystem.h"
#include "VisualDebugSystem.h"

#include "AddToEntityMessage.h"

BuilderSceneECS::BuilderSceneECS()
: Scene("BuilderSceneECS")
, map_file_(new FileChannel("pkmn_map_test.txt"))
{
}

BuilderSceneECS::~BuilderSceneECS() {
   delete this->map_file_;
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

   // TODO: there's got to be a better way to do this
   gs->subscription(new SpatialEntitySubscription(gs->id() + "EntitySubscription", map_root->handle()));

   GraphicalSystem* hud_graphics = new GraphicalSystem("HudGraphics", game.window(), std::make_shared<Camera>("HudCamera"));
   hud_graphics->subscription(new SpatialEntitySubscription(hud_graphics->id() + "EntitySubscription", hud_root->handle()));
   this->add_system(hud_graphics);

   this->add_system(new VisualDebugSystem("VisualDebugSystemLeft", game.window(), gs->camera()));

   // create mouse cursor
   Entity* mouse_cursor = this->get_entity(this->create_entity());
   mouse_cursor->id("MouseCursorEntity");

   // add mouse_cursor to the hud_root
   this->send_message_async<AddToEntityMessage>(hud_root->handle(), mouse_cursor->handle());

   mouse_cursor->add<Clickable>("MouseCursorClickable");
   mouse_cursor->add<Collision>("MouseCursorCollision", sf::FloatRect(0, 0, 6, 6));

   mouse_cursor->add<Rectangle>("MouseCursorRectangle", 0, 0, 6, 6);
   mouse_cursor->get<Rectangle>()->color(sf::Color::Red);

   mouse_cursor->add<PlayerProfile>("MouseCursorPlayerProfile", 1);

   Entity* mouse_cursor_text = this->get_entity(this->create_entity());
   mouse_cursor_text->id("MouseCursorTextEntity");

   mouse_cursor_text->add<Text>("MouseCursorText", "0, 0", this->fonts().get("retro"), 12);
   mouse_cursor_text->get<Text>()->position(0, 10);
   mouse_cursor_text->get<Text>()->color(sf::Color::White);

   this->send_message_async<AddToEntityMessage>(mouse_cursor->handle(), mouse_cursor_text->handle());

   // define mouse cursor behavior
   mouse_cursor->add<Callback>("MouseCursorCallback");
   mouse_cursor->get<Callback>()->mouse_move([mouse_cursor, mouse_cursor_text, &game] () {
      sf::Vector2f new_pos;
      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      mouse_cursor->get<Rectangle>()->position(new_pos - sf::Vector2f(3, 3));

      mouse_cursor_text->get<Text>()->position(new_pos + sf::Vector2f(0, 10));
      mouse_cursor_text->get<Text>()->string(
         std::to_string(static_cast<int>(new_pos.x)) +
         ", " +
         std::to_string(static_cast<int>(new_pos.y))
      );

      Collision* c = mouse_cursor->get<Collision>();
      if (c) {
         c->volume(new_pos - sf::Vector2f(3, 3), sf::Vector2f(6, 6));
      }
   });

   mouse_cursor->get<Callback>()->mouse_wheel([&game] () {
      float wheel_delta = game.get_player(1).bindings().get<MouseWheelIntent>()->element()->position();
      Game::logger().msg("asdf", Logger::INFO, "Mouse delta " + std::to_string(wheel_delta));
   });

   mouse_cursor->get<Callback>()->left_click([] () {
      Game::logger().msg("asdf", Logger::INFO, "LEFT CLICK");
   });

   mouse_cursor->get<Callback>()->left_release([] () {
      Game::logger().msg("asdf", Logger::INFO, "LEFT RELEASE");
   });

   mouse_cursor->get<Callback>()->right_click([] () {
      Game::logger().msg("asdf", Logger::INFO, "RIGHT CLICK");
   });

   mouse_cursor->get<Callback>()->right_release([] () {
      Game::logger().msg("asdf", Logger::INFO, "RIGHT RELEASE");
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
