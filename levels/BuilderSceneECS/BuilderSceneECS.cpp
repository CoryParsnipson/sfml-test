#include "sfml.h"

#include "BuilderSceneECS.h"

#include "Game.h"
#include "Text.h"
#include "Rectangle.h"
#include "Callback.h"
#include "Entity.h"
#include "PlayerProfile.h"

#include "FileChannel.h"

#include "MouseXIntent.h"
#include "MouseYIntent.h"

#include "SpatialSystem.h"
#include "GraphicalSystem.h"
#include "CallbackSystem.h"

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

   // add in GraphicalSystem (this probably should go in Scene.h or something...)
   GraphicalSystem* gs = new GraphicalSystem("GraphicalSystem", game.window(), std::make_shared<Camera>("Main Camera"));
   gs->camera()->reset_pan();
   this->add_system(gs);

   // add in SpatialSystem (this probably should go in Scene.h or something...)
   this->add_system(new SpatialSystem());

   // add in CallbackSystem (this probably should go in Scene.h or something...)
   this->add_system(new CallbackSystem());

   // create mouse cursor
   Entity* mouse_cursor = this->get_entity(this->create_entity());
   mouse_cursor->id("MouseCursorEntity");

   mouse_cursor->add<Rectangle>();
   mouse_cursor->get<Rectangle>()->position(0, 0);
   mouse_cursor->get<Rectangle>()->size(6, 6);
   mouse_cursor->get<Rectangle>()->color(sf::Color::Red);

   mouse_cursor->add<Text>();
   mouse_cursor->get<Text>()->position(0, 10);
   mouse_cursor->get<Text>()->font_size(12);
   mouse_cursor->get<Text>()->color(sf::Color::White);
   mouse_cursor->get<Text>()->font(this->fonts().get("retro"));

   mouse_cursor->add<PlayerProfile>("PlayerProfile Component", 1);

   // define mouse cursor behavior
   mouse_cursor->add<Callback>();
   mouse_cursor->get<Callback>()->mouse_move([mouse_cursor, &game] () {
      sf::Vector2f new_pos;

      new_pos.x = game.get_player(1).bindings().get<MouseXIntent>()->element()->position();
      new_pos.y = game.get_player(1).bindings().get<MouseYIntent>()->element()->position();

      mouse_cursor->get<Rectangle>()->position(new_pos);
   });

   mouse_cursor->get<Callback>()->left_click([] () {
   });

   mouse_cursor->get<Callback>()->left_release([] () {
   });

   mouse_cursor->get<Callback>()->right_click([] () {
   });

   mouse_cursor->get<Callback>()->right_release([] () {
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
