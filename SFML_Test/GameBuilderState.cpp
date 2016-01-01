#include "GameBuilderState.h"

#include "TextSerializer.h"

#include "IsoMapBuilder.h"
#include "FlatMapBuilder.h"

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
   game.texture_manager.create_texture("tile_solid", "flatmap_test_texture.png", sf::IntRect(0, 0, 40, 40));
   game.texture_manager.create_texture("tile_clear", "flatmap_test_texture.png", sf::IntRect(40, 0, 40, 40));
   game.texture_manager.print();

   TextSerializer* serializer = new TextSerializer();
   serializer->open("flat_map_test.txt");

   // build the map
   //MapBuilder* map_builder = new IsoMapBuilder(game.texture_manager);
   //map_builder->set_serializer(serializer);
   //map_builder->build();

   //this->map = map_builder->get_map();

   MapBuilder* map_builder = new FlatMapBuilder(game.texture_manager);
   map_builder->set_serializer(serializer);
   map_builder->build();

   this->map = map_builder->get_map();

   delete map_builder;
   delete serializer;

   // entities
   this->e = UtilFactory::inst()->create_mouse(this->viewports["hud"]);
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->e->get("control")));

   this->origin_dot = new sf::RectangleShape(sf::Vector2f(3, 3));
   this->origin_dot->setFillColor(sf::Color::Yellow);

   this->center_dot = new sf::RectangleShape(sf::Vector2f(3, 3));
   this->center_dot->setFillColor(sf::Color(255, 104, 2));
   this->center_dot->setPosition(Settings::Instance()->SCREEN_WIDTH / 2, Settings::Instance()->SCREEN_HEIGHT / 2);

   for (int r = 0; r < 40; r++) {
      for (int c = 0; c < 40; c++) {
         sf::RectangleShape* g = new sf::RectangleShape(sf::Vector2f(3, 3));
         g->setFillColor(sf::Color::Blue);
         g->setPosition(r * Settings::Instance()->TILE_WIDTH, c * Settings::Instance()->TILE_HEIGHT);

         this->grid.push_back(g);
      }
   }
}

void GameBuilderState::exit(Game& game) {
   delete this->e;
   delete this->origin_dot;
   delete this->center_dot;

   this->viewports.clear();
   this->grid.clear();
}

GameState* GameBuilderState::update(Game& game) {
   // map related work
   this->map->update(game, *this->viewports["main"]);
   
   // update entities
   this->e->update(*this->viewports["main"]);

   //std::vector<sf::RectangleShape*>::const_iterator grid_it;
   //for (grid_it = this->grid.begin(); grid_it != this->grid.end(); grid_it++) {
   //   this->viewports["main"]->draw(**grid_it);
   //}

   this->viewports["hud"]->draw(*this->center_dot);
   //this->viewports["main"]->draw(*this->origin_dot);

   // draw fixed hud items
   this->viewports["hud"]->write("SFML_Test");
   this->viewports["hud"]->write("r: reset pan position", sf::Vector2f(0, 15));
   this->viewports["hud"]->write("right click: click and drag to pan", sf::Vector2f(0, 30));

   // show FPS
   FontConfig fc("retro", 12, FontConfig::ALIGN::RIGHT);
   int fps = (int)(1.f / this->clock.getElapsedTime().asSeconds());
   this->clock.restart();
   this->viewports["hud"]->write("FPS: " + std::to_string(fps), sf::Vector2f(Settings::Instance()->SCREEN_WIDTH - 4, 10), &fc);

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
