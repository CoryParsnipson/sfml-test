#include "BuilderScene.h"

#include "Game.h"
#include "Viewport.h"

#include "Entity.h"
#include "GraphicsPart.h"

#include "UtilFactory.h"
#include "TextSerializer.h"
#include "FlatMapBuilder.h"

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg("BuilderScene", Logger::INFO, "Entering builder start menu state.");

   this->viewports_["main"] = new Viewport(game.graphics, static_cast<sf::Vector2f>(game.graphics.get_window().getSize()));
   this->viewports_["hud"] = new Viewport(game.graphics, static_cast<sf::Vector2f>(game.graphics.get_window().getSize()));

   // load textures
   game.texture_manager.create_texture("tile_solid", "flatmap_test_texture.png", sf::IntRect(0, 0, 40, 40));
   game.texture_manager.create_texture("tile_clear", "flatmap_test_texture.png", sf::IntRect(40, 0, 40, 40));
   game.texture_manager.print();

   TextSerializer* serializer = new TextSerializer();
   serializer->open("flat_map_test.txt");

   // build the map
   MapBuilder* map_builder = new FlatMapBuilder(game.texture_manager);
   map_builder->set_serializer(serializer);
   map_builder->build();

   this->map = map_builder->get_map();

   delete map_builder;
   delete serializer;

   // entities
   this->e = UtilFactory::inst()->create_mouse(this->viewports_["hud"], this->viewports_["main"]);
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->e->get("control")));
   
   GraphicsPart* mouse_graphics = dynamic_cast<GraphicsPart*>(this->e->get("graphics"));
   mouse_graphics->set_show_debug_text(true);

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

void BuilderScene::exit(Game& game) {
   delete this->e;
   delete this->origin_dot;
   delete this->center_dot;

   this->viewports_.clear();
   this->grid.clear();
}

void BuilderScene::update(Game& game) {
   // map related work
   this->map->update(game, *this->viewports_["main"]);
   
   // update entities
   this->e->update(game);

   //std::vector<sf::RectangleShape*>::const_iterator grid_it;
   //for (grid_it = this->grid.begin(); grid_it != this->grid.end(); grid_it++) {
   //   this->viewports_["main"]->draw(**grid_it);
   //}

   this->viewports_["hud"]->draw(*this->center_dot);
   //this->viewports_["main"]->draw(*this->origin_dot);

   // draw fixed hud items
   this->viewports_["hud"]->write("SFML_Test");
   this->viewports_["hud"]->write("r: reset pan position", sf::Vector2f(0, 15));
   this->viewports_["hud"]->write("right click: click and drag to pan", sf::Vector2f(0, 30));

   // show FPS
   FontConfig fc("retro", 12, FontConfig::ALIGN::RIGHT);
   int fps = (int)(1.f / this->clock.getElapsedTime().asSeconds());
   this->clock.restart();
   this->viewports_["hud"]->write("FPS: " + std::to_string(fps), sf::Vector2f(Settings::Instance()->SCREEN_WIDTH - 4, 10), &fc);
}

void BuilderScene::process(Game& game, CloseCommand& c) {
   game.unload_scene();
}

void BuilderScene::process(Game& game, KeyPressCommand& c) {
   sf::Vector2f delta;

   switch (c.event.code) {
   case sf::Keyboard::Key::R:
      this->viewports_["main"]->set_center(this->viewports_["main"]->get_default_center());
      this->viewports_["main"]->set_size(this->viewports_["main"]->get_default_size());
      this->viewports_["main"]->set_zoom_factor(1.0);
   break;
   default:
      // do nothing
   break;
   }
}

void BuilderScene::process(Game& game, WindowResizeCommand& c) {
   std::map<std::string, Viewport*>::iterator it;
   
   sf::Vector2f screen_size((float)c.width, (float)c.height);
   sf::Vector2f screen_center(c.width / 2.f, c.height / 2.f);
   
   // keep hud viewport fixed over render window
   this->viewports_["hud"]->set_size(screen_size);
   this->viewports_["hud"]->set_center(screen_center);

   this->viewports_["main"]->set_size(screen_size);
   this->viewports_["main"]->set_default_size(screen_size);
}

void BuilderScene::process(Game& game, MouseButtonCommand& c) {
   if (c.button != MouseButtonCommand::MOUSE_BUTTON::LEFT) {
      return;
   }

   sf::Vector2i mouse_pos(c.x, c.y);
}

void BuilderScene::process(Game& game, MouseMoveCommand& c) {}
void BuilderScene::process(Game& game, MouseWheelCommand& c) {}
