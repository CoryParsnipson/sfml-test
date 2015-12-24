#include "GameBuilderState.h"

#include "UtilFactory.h"
#include "TileFactory.h"

#include "Part.h"
#include "GraphicsPart.h"
#include "PhysicsPart.h"

#include "Game.h"
#include "Graphics.h"

// initialize static instance
GameBuilderState GameState::builder_state;

void GameBuilderState::enter(Game& game) {
   Service::get_logger().msg("GameBuilderState", Logger::INFO, "Entering builder start menu state.");

   this->viewports["main"] = new Viewport(game.graphics, static_cast<sf::Vector2f>(game.graphics.get_window().getSize()));
   this->viewports["hud"] = new Viewport(game.graphics, static_cast<sf::Vector2f>(game.graphics.get_window().getSize()));

   // load textures
   game.texture_manager.create_texture(0, "tile1.gif");
   game.texture_manager.create_texture(1, "tile_grass.gif");
   game.texture_manager.print();

   // entities
   //this->map = new Map();
   //this->map->load_mapfile(game, "map_test.txt");

   this->e = UtilFactory::inst()->create_mouse(this->viewports["hud"]);
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->e->get("control")));

   // make some tiles, bitch
   this->tiles.push_back(TileFactory::inst()->create_tile(*game.texture_manager.get_texture(0), sf::Vector2f(240, 166))); 
   this->tiles.push_back(TileFactory::inst()->create_tile(*game.texture_manager.get_texture(0), sf::Vector2f(160, 566))); 
   this->tiles.push_back(TileFactory::inst()->create_tile(*game.texture_manager.get_texture(0), sf::Vector2f(555, 333))); 
}

void GameBuilderState::exit(Game& game) {
}

GameState* GameBuilderState::update(Game& game) {
   // draw map view items
   //this->map->draw(*this->viewports["main"]);

   // update entities
   this->e->update(*this->viewports["main"]);

   std::vector<Entity2*>::const_iterator it;
   for (it = this->tiles.begin(); it != this->tiles.end(); it++) {
      (*it)->update(*this->viewports["main"]);
   }

   // draw fixed hud items
   this->viewports["hud"]->write("SFML_Test");
   this->viewports["hud"]->write("r: reset pan position", sf::Vector2f(0, 15));
   this->viewports["hud"]->write("right click: click and drag to pan", sf::Vector2f(0, 30));

   return NULL;
}

void GameBuilderState::process(Game& game, CloseCommand& c) {
   Service::get_logger().msg("GameBuilderState", Logger::INFO, "close command received.");
   game.exit();
}

void GameBuilderState::process(Game& game, KeyPressCommand& c) {
   sf::Vector2f delta;

   switch (c.event.code) {
   case sf::Keyboard::Key::R:
      this->viewports["main"]->set_center(this->viewports["main"]->get_default_center());
      this->viewports["main"]->set_size(this->viewports["main"]->get_default_size());
      this->viewports["main"]->set_zoom_factor(1.0);
   break;
   default:
      // do nothing
   break;
   }
}

void GameBuilderState::process(Game& game, WindowResizeCommand& c) {
   std::map<std::string, Viewport*>::iterator it;
   
   sf::Vector2f screen_size((float)c.width, (float)c.height);
   sf::Vector2f screen_center(c.width / 2.f, c.height / 2.f);
   
   // keep hud viewport fixed over render window
   this->viewports["hud"]->set_size(screen_size);
   this->viewports["hud"]->set_center(screen_center);

   this->viewports["main"]->set_size(screen_size);
   this->viewports["main"]->set_default_size(screen_size);
}

void GameBuilderState::process(Game& game, MouseButtonCommand& c) {
   if (c.button != MouseButtonCommand::MOUSE_BUTTON::LEFT) {
      return;
   }

   sf::Vector2i mouse_pos(c.x, c.y);
}

void GameBuilderState::process(Game& game, MouseMoveCommand& c) {}
void GameBuilderState::process(Game& game, MouseWheelCommand& c) {}
