#include "BuilderScene.h"

#include "Game.h"
#include "Viewport.h"

#include "Entity.h"
#include "PhysicsPart.h"
#include "MouseControlPart.h"

#include "UtilFactory.h"
#include "TileFactory.h"
#include "TextSerializer.h"
#include "FlatMapBuilder.h"

BuilderScene::BuilderScene()
: click_press_pos(nullptr)
, click_release_pos(nullptr)
, selected_tile(nullptr)
, fps_font("retro", 12, FontConfig::ALIGN::LEFT)
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
{
}

BuilderScene::~BuilderScene() {
   delete this->map;
   delete this->map_builder;

   delete this->click_press_pos;
   delete this->click_release_pos;

   delete this->e;
   delete this->selected_tile;

   delete this->origin_dot;
   delete this->center_dot;

   std::vector<sf::RectangleShape*>::const_iterator it;
   for (it = this->grid.begin(); it != this->grid.end(); ++it) {
      delete *it;
   }
   this->grid.clear();
}

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
   this->map_builder = new FlatMapBuilder(game.texture_manager);
   this->map_builder->set_serializer(serializer);
   this->map_builder->build();

   this->map = this->map_builder->get_map();

   delete map_builder;
   delete serializer;

   // entities
   this->e = UtilFactory::inst()->create_mouse();
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->e->get("control")));

   // let our mouse controller manipulate this scene
   dynamic_cast<MouseControlPart*>(this->e->get("control"))->set_controllable(this);
   
   this->origin_dot = new sf::RectangleShape(sf::Vector2f(3, 3));
   this->origin_dot->setFillColor(sf::Color::Yellow);

   this->center_dot = new sf::RectangleShape(sf::Vector2f(3, 3));
   this->center_dot->setFillColor(sf::Color(255, 104, 2));
   this->center_dot->setPosition(Settings::Instance()->SCREEN_WIDTH / 2, Settings::Instance()->SCREEN_HEIGHT / 2);

   for (int r = 0; r < 40; r++) {
      for (int c = 0; c < 40; c++) {
         sf::RectangleShape* g = new sf::RectangleShape(sf::Vector2f(2, 2));
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
   this->map->update(*this, *this->viewports_["main"]);
   
   // update entities
   if (this->selected_tile) {
      this->selected_tile->update(*this, *this->viewports_["main"]);
   }

   std::vector<Entity*>::const_iterator entity_it;
   for (entity_it = this->entities_.begin(); entity_it != this->entities_.end(); ++entity_it) {
      (*entity_it)->update(*this, *this->viewports_["main"]);
   }

   std::vector<sf::RectangleShape*>::const_iterator grid_it;
   for (grid_it = this->grid.begin(); grid_it != this->grid.end(); grid_it++) {
      this->viewports_["main"]->draw(**grid_it);
   }

   this->viewports_["main"]->draw(*this->origin_dot);
   this->viewports_["hud"]->draw(*this->center_dot);

   this->e->update(*this, *this->viewports_["hud"]);

   // draw fixed hud items
   this->viewports_["hud"]->write("SFML_Test");
   this->viewports_["hud"]->write("r: reset pan position", sf::Vector2f(0, 15));
   this->viewports_["hud"]->write("right click: click and drag to pan", sf::Vector2f(0, 30));

   // calculate and show FPS
   if (!this->frame_count) {
      this->last_frame_time = (((float)this->frame_measurement_interval / this->clock.getElapsedTime().asSeconds()) * Settings::Instance()->FRAMERATE_SMOOTHING)
                              + (this->last_frame_time * (1.0 - Settings::Instance()->FRAMERATE_SMOOTHING));
      this->clock.restart();
   }

   sf::Vector2f fps_pos(this->viewports_["hud"]->get_size().x - 60, 8);
   this->viewports_["hud"]->write("FPS: " + std::to_string(this->last_frame_time), fps_pos, &this->fps_font);
   this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
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
      this->viewports_["main"]->set_scale(1.0);
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

   // reposition center dot
   this->center_dot->setPosition(screen_center.x, screen_center.y);
}

void BuilderScene::process(Game& game, MouseButtonCommand& c) {
   if (c.button != MouseButtonCommand::MOUSE_BUTTON::LEFT) {
      return;
   }

   sf::Vector2i mouse_pos(c.x, c.y);
}

void BuilderScene::process(Game& game, MouseMoveCommand& c) {}
void BuilderScene::process(Game& game, MouseWheelCommand& c) {}

void BuilderScene::drag(sf::Vector2f delta) {
   try {
      if (this->click_press_pos && !this->click_release_pos) {
         // TODO: probably need update() on Viewport class to get this working properly...
         sf::RectangleShape select_rect;
         select_rect.setPosition(*this->click_press_pos);
         select_rect.setSize(*this->click_press_pos + delta);
         select_rect.setFillColor(sf::Color(246, 168, 104));
         select_rect.setOutlineColor(sf::Color(191, 127, 63));
         select_rect.setOutlineThickness(1.0);

         this->viewports_.at("main")->draw(select_rect);
      } else {
         this->viewports_.at("main")->drag(delta);
      }
   }
   catch (const std::out_of_range& e) {
      Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
      return; // not sure if this is necessary...
   }
}

float BuilderScene::get_scale() {
   try {
      return this->viewports_.at("main")->get_scale();
   }
   catch (const std::out_of_range& e) {
      Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
      return 1.0; // not sure if this is necessary...
   }
}

void BuilderScene::set_scale(float factor) {
   try {
      this->viewports_.at("main")->set_scale(factor);
   }
   catch (const std::out_of_range& e) {
      Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
      return; // not sure if this is necessary...
   }
}

void BuilderScene::click(MouseButtonCommand& c) {
   sf::Vector2f world_coord;

   try {
      world_coord = this->viewports_.at("main")->get_world_coord(sf::Vector2i(c.x, c.y));
   }
   catch (const std::out_of_range& e) {
      Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
      return; // not sure if this is necessary...
   }

   // on right click, begin select behavior
   if (c.button == MouseButtonCommand::MOUSE_BUTTON::LEFT && c.state == MouseButtonCommand::STATE::PRESSED) {
      this->click_press_pos = new sf::Vector2f(world_coord);
   }

   // on left click, select a map tile
   if (c.button == MouseButtonCommand::MOUSE_BUTTON::LEFT && c.state == MouseButtonCommand::STATE::RELEASED) {
      Map::tiles_t tiles;
      bool is_one_tile_selected = false;

      Service::get_logger().msg("BuilderScene", Logger::INFO, "world_coord: (" + std::to_string(world_coord.x) + ", " + std::to_string(world_coord.y) + ")");

      this->click_release_pos = new sf::Vector2f(world_coord);

      is_one_tile_selected = (std::abs(this->click_press_pos->x - this->click_release_pos->x) < 10) &&
         (std::abs(this->click_press_pos->y - this->click_release_pos->y) < 10);

      Service::get_logger().msg("BuilderScene", Logger::INFO, "is_one_tile_selected? " + std::to_string(is_one_tile_selected));

      // TODO: revised tile cursor select
      // * figure out how to create new tiles for areas under selection rectangle not already in map
      
      // round press and release click positions to tiles
      this->click_press_pos->x = Settings::Instance()->TILE_WIDTH * (int)(this->click_press_pos->x / Settings::Instance()->TILE_WIDTH);
      this->click_press_pos->y = Settings::Instance()->TILE_HEIGHT * (int)(this->click_press_pos->y / Settings::Instance()->TILE_HEIGHT);

      this->click_release_pos->x = Settings::Instance()->TILE_WIDTH * ((int)(this->click_release_pos->x / Settings::Instance()->TILE_WIDTH) + ((int)this->click_release_pos->x % (int)Settings::Instance()->TILE_WIDTH ? 1 : 0));
      this->click_release_pos->y = Settings::Instance()->TILE_HEIGHT * ((int)(this->click_release_pos->y / Settings::Instance()->TILE_HEIGHT) + ((int)this->click_release_pos->y % (int)Settings::Instance()->TILE_HEIGHT ? 1 : 0));

      sf::Vector2f* click_origin_pos = this->click_press_pos;
      if (this->click_release_pos->x < this->click_press_pos->x || (this->click_release_pos->x == this->click_press_pos->x && this->click_release_pos->y < this->click_press_pos->y)) {
         click_origin_pos = this->click_release_pos;
      }

      sf::Vector2f selected_size(*this->click_release_pos - *this->click_press_pos);
      selected_size.x = std::max(selected_size.x, Settings::Instance()->TILE_WIDTH);
      selected_size.y = std::max(selected_size.y, Settings::Instance()->TILE_HEIGHT);

      sf::FloatRect selected(*click_origin_pos, selected_size);
      tiles = this->map->intersects(selected);

      delete this->click_press_pos;
      delete this->click_release_pos;

      this->click_press_pos = nullptr;
      this->click_release_pos = nullptr;

      if (this->selected_tile) {
         bool selected_is_one_tile = false;
         PhysicsPart* selected_physics = dynamic_cast<PhysicsPart*>(this->selected_tile->get("physics"));
         
         if (!selected_physics) {
            delete this->selected_tile;
            this->selected_tile = nullptr;

            Service::get_logger().msg("BuilderScene", Logger::WARNING, "Tile cursor does not have physics component.");
            return;
         }

         selected_is_one_tile = ((int)selected_physics->get_size().x == (int)Settings::Instance()->TILE_WIDTH);
         Service::get_logger().msg("BuilderScene", Logger::INFO, "selected_is_one_tile? " + std::to_string(selected_is_one_tile));

         // TODO: wow this is terrible... "selected_is_one_tile" is about the existing cursor and "is_one_tile_selected" is about the mouse drag selection
         if (!selected_physics->intersects(world_coord) && ((!selected_is_one_tile && !is_one_tile_selected) ||
                                                            (selected_is_one_tile && !is_one_tile_selected) ||
                                                            (selected_is_one_tile && is_one_tile_selected))) {
            delete this->selected_tile;
            this->selected_tile = TileFactory::inst()->create_tile_cursor(sf::Vector2f(selected.left, selected.top), sf::Vector2f(selected.width, selected.height), tiles);
         } else {
            delete this->selected_tile;
            this->selected_tile = nullptr;
         }
      } else {
         this->selected_tile = TileFactory::inst()->create_tile_cursor(sf::Vector2f(selected.left, selected.top), sf::Vector2f(selected.width, selected.height), tiles);
      }
   }
}
