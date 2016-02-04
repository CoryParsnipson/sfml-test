#include "BuilderScene.h"

#include "Game.h"
#include "Layer.h"

#include "Entity.h"
#include "GraphicsPart.h"
#include "PhysicsPart.h"
#include "ReferencePart.h"
#include "MouseControlPart.h"

#include "UtilFactory.h"
#include "TileFactory.h"
#include "TextFactory.h"
#include "TextSerializer.h"
#include "FlatMapBuilder.h"

BuilderScene::BuilderScene()
: map_(nullptr)
, center_dot_(nullptr)
, mouse_(nullptr)
, selection_rectangle_(nullptr)
, tile_cursor_(nullptr)
, reset_size_(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT)
, reset_center_(reset_size_.x / 2.f, reset_size_.y / 2.f)
, last_mouse_pos_(nullptr)

, click_press_pos_(nullptr)
, click_release_pos_(nullptr)
//, fps_font("retro", 12, FontConfig::ALIGN::LEFT)
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
{
}

BuilderScene::~BuilderScene() {
   delete this->map_;

   //delete this->click_press_pos_;
   //delete this->click_release_pos_;

   //std::vector<sf::RectangleShape*>::const_iterator it;
   //for (it = this->grid.begin(); it != this->grid.end(); ++it) {
   //   delete *it;
   //}
   //this->grid.clear();
}

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg("BuilderScene", Logger::INFO, "Entering builder start menu state.");
   
   // create viewport(s)
   this->viewport_ = new Viewport(this->reset_size_);

   // fixed layer above map and sprites
   this->viewport_->add("hud");

   // load textures
   game.texture_manager.create_texture("tile_solid", "flatmap_test_texture.png", sf::IntRect(0, 0, 40, 40));
   game.texture_manager.create_texture("tile_clear", "flatmap_test_texture.png", sf::IntRect(40, 0, 40, 40));
   game.texture_manager.print();

   // build the map
   TextSerializer* serializer = new TextSerializer();
   serializer->open("flat_map_test.txt");

   MapBuilder* map_builder = new FlatMapBuilder(game.texture_manager);
   map_builder->set_serializer(serializer);
   map_builder->build();

   this->map_ = map_builder->get_map();
   this->viewport_->get("main")->add(this->map_); // add map to draw list

   delete serializer;
   delete map_builder;

   // initialize entities
   this->mouse_ = UtilFactory::inst()->create_mouse();
   this->entities_.push_back(this->mouse_);
   this->viewport_->get("hud")->add(this->mouse_);

   // hook up mouse controller to user input
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->mouse_->get("control")));

   // let our mouse controller manipulate this scene
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(this);
   
   // create fixed hud items
   Entity* t = TextFactory::inst()->create_text("SFML_Test", "retro");
   this->viewport_->get("hud")->add(t);
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text("r: reset pan position", "retro", sf::Vector2f(0, 15));
   this->viewport_->get("hud")->add(t);
   this->entities_.push_back(t);
   
   t = TextFactory::inst()->create_text("right click: click and drag to pan", "retro", sf::Vector2f(0, 30));
   this->viewport_->get("hud")->add(t);
   this->entities_.push_back(t);

   sf::RectangleShape* origin_dot = new sf::RectangleShape(sf::Vector2f(3, 3));
   origin_dot->setFillColor(sf::Color::Yellow);
   Entity* origin_dot_entity = UtilFactory::inst()->create_graphic(origin_dot);

   this->entities_.push_back(origin_dot_entity);
   this->viewport_->get("main")->add(origin_dot_entity);

   sf::RectangleShape* center_dot_graphic = new sf::RectangleShape(sf::Vector2f(3, 3));
   center_dot_graphic->setFillColor(sf::Color(255, 104, 2));
   center_dot_graphic->setPosition(Settings::Instance()->SCREEN_WIDTH / 2, Settings::Instance()->SCREEN_HEIGHT / 2);
   this->center_dot_ = UtilFactory::inst()->create_graphic(center_dot_graphic);

   this->center_dot_->add(new PhysicsPart("physics", center_dot_graphic->getGlobalBounds()));

   this->entities_.push_back(this->center_dot_);
   this->viewport_->get("hud")->add(this->center_dot_);

   //for (int r = 0; r < 40; r++) {
   //   for (int c = 0; c < 40; c++) {
   //      sf::RectangleShape* g = new sf::RectangleShape(sf::Vector2f(2, 2));
   //      g->setFillColor(sf::Color::Blue);
   //      g->setPosition(r * Settings::Instance()->TILE_WIDTH, c * Settings::Instance()->TILE_HEIGHT);

   //      this->grid.push_back(g);
   //   }
   //}
}

void BuilderScene::exit(Game& game) {
}

void BuilderScene::draw(Graphics& graphics) {
   Scene::draw(graphics);
}

void BuilderScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);

   // map related work
   this->map_->update(game, this);
   
   // update entities
   if (this->click_press_pos_ && this->last_mouse_pos_) {
      sf::Vector2f origin_pos;
      sf::Vector2f end_pos;

      origin_pos.x = std::min(this->click_press_pos_->x, this->last_mouse_pos_->x);
      origin_pos.y = std::min(this->click_press_pos_->y, this->last_mouse_pos_->y);

      end_pos.x = std::max(this->click_press_pos_->x, this->last_mouse_pos_->x);
      end_pos.y = std::max(this->click_press_pos_->y, this->last_mouse_pos_->y);

      if (!this->selection_rectangle_) {
         sf::RectangleShape* sr = new sf::RectangleShape();
         sr->setFillColor(sf::Color(66, 108, 167, 175));
         sr->setOutlineColor(sf::Color(124, 160, 210, 192));
         sr->setOutlineThickness(1.0);

         this->selection_rectangle_ = UtilFactory::inst()->create_graphic(sr);

         this->entities_.push_back(this->selection_rectangle_);
         this->viewport_->get("main")->add(this->selection_rectangle_);
      }

      // update position and size of selection rectangle
      GraphicsPart* sr_graphics = dynamic_cast<GraphicsPart*>(this->selection_rectangle_->get("graphics"));
      sf::RectangleShape* sr = dynamic_cast<sf::RectangleShape*>(sr_graphics->get(0));

      sr->setPosition(origin_pos);
      sr->setSize(end_pos - origin_pos);
   }


   //if (this->selected_tile) {
   //   this->selected_tile->update(game, this);
   //   this->selected_tile->draw(*this->viewports_["main"]);
   //}

   //std::vector<sf::RectangleShape*>::const_iterator grid_it;
   //for (grid_it = this->grid.begin(); grid_it != this->grid.end(); grid_it++) {
   //   this->viewports_["main"]->draw(**grid_it);
   //}

   //// calculate and show FPS
   //if (!this->frame_count) {
   //   this->last_frame_time = (((float)this->frame_measurement_interval / this->clock.getElapsedTime().asSeconds()) * Settings::Instance()->FRAMERATE_SMOOTHING)
   //                           + (this->last_frame_time * (1.0 - Settings::Instance()->FRAMERATE_SMOOTHING));
   //   this->clock.restart();
   //}

   //sf::Vector2f fps_pos(this->viewports_["hud"]->get_size().x - 60, 8);
   //this->viewports_["hud"]->write("FPS: " + std::to_string(this->last_frame_time), fps_pos, &this->fps_font);
   //this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
}

void BuilderScene::process(Game& game, CloseCommand& c) {
   game.unload_scene();
}

void BuilderScene::process(Game& game, KeyPressCommand& c) {
   sf::Vector2f delta;
   Map::tile_grid_t::const_iterator it;

   switch (c.event.code) {
   case sf::Keyboard::Key::R:
      this->viewport_->get("main")->set_size(this->reset_size_);
      this->viewport_->get("main")->set_center(this->reset_center_);
      this->viewport_->get("main")->set_scale(1.0);
   break;
   case sf::Keyboard::Key::F:
      //if (this->selected_tile) {
      //   // TODO: refactor when infrastructure for this is up
      //   Service::get_logger().msg("BuilderScene", Logger::INFO, "Adding new tiles.");

      //   PhysicsPart* selected_physics = dynamic_cast<PhysicsPart*>(this->selected_tile->get("physics"));
      //   if (!selected_physics) {
      //      Service::get_logger().msg("BuilderScene", Logger::WARNING, "In process KeyPressCommand (F) -> selected tile cursor has no physics component.");
      //      return;
      //   }

      //   // split selected tile area into tile-sized chunks
      //   sf::FloatRect proto_tile_bounds(selected_physics->get_position(), sf::Vector2f(Settings::Instance()->TILE_WIDTH, Settings::Instance()->TILE_HEIGHT));
      //   float next_x = selected_physics->get_position().x;
      //   float next_y = selected_physics->get_position().y;

      //   int idx_x = (int)(next_x / Settings::Instance()->TILE_WIDTH);
      //   int idx_y = (int)(next_y / Settings::Instance()->TILE_HEIGHT);

      //   float max_x = next_x + selected_physics->get_size().x;
      //   float max_y = next_y + selected_physics->get_size().y;
      //   
      //   while (next_y < max_y) {
      //      while (next_x < max_x) {
      //         // figure out which tile chunks already exist in map
      //         Service::get_logger().msg("BuilderScene", Logger::INFO, "checking tile " + std::to_string(next_x) + ", " + std::to_string(next_y));

      //         if (this->map->intersects(proto_tile_bounds).size() == 0) {
      //            Entity* tile = TileFactory::inst()->create_tile(game.texture_manager.get_texture("tile_solid"), sf::Vector2f(next_x, next_y));
      //            ReferencePart* ref_part = dynamic_cast<ReferencePart*>(this->selected_tile->get("reference"));

      //            // create new tiles
      //            this->map->add(idx_x, idx_y, tile);

      //            // add them to the cursor
      //            if (ref_part) {
      //               ref_part->add(tile);
      //            }
      //         } else {
      //            // TODO: modify existing tiles
      //            Service::get_logger().msg("BuilderScene", Logger::INFO, "poop");
      //         }

      //         idx_x += 1;
      //         next_x += Settings::Instance()->TILE_WIDTH;
   
      //         proto_tile_bounds.left = next_x;
      //         proto_tile_bounds.top = next_y;
      //      }

      //      idx_y += 1;
      //      next_y += Settings::Instance()->TILE_HEIGHT;
      //      
      //      next_x = selected_physics->get_position().x;
      //      idx_x = (int)(next_x / Settings::Instance()->TILE_WIDTH);
      //   }
      //}
   break;
   case sf::Keyboard::Key::D:
      //// toggle debug text on all tiles
      //for (it = this->map->get_tiles().begin(); it != this->map->get_tiles().end(); ++it) {
      //   GraphicsPart* tile_graphics = dynamic_cast<GraphicsPart*>(it->second->get("graphics"));

      //   if (tile_graphics) {
      //      tile_graphics->set_show_debug_text(!tile_graphics->get_show_debug_text());
      //   }
      //}
   break;
   default:
      // do nothing
   break;
   }
}

void BuilderScene::process(Game& game, WindowResizeCommand& c) {
   sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

   this->viewport_->resize(sf::Vector2f((float)c.width, (float)c.height));
   this->viewport_->recenter(new_center);

   // reposition center dot
   PhysicsPart* center_dot_physics = dynamic_cast<PhysicsPart*>(this->center_dot_->get("physics"));
   if (center_dot_physics) {
      center_dot_physics->set_position(new_center);
   }
}

void BuilderScene::process(Game& game, MouseButtonCommand& c) {}
void BuilderScene::process(Game& game, MouseMoveCommand& c) {}
void BuilderScene::process(Game& game, MouseWheelCommand& c) {}

void BuilderScene::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   Layer* main_layer = this->viewport_->get("main");
   if (!main_layer) {
      Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
      return;
   }

   if (c.button == MouseButtonCommand::LEFT) {
      // update last mouse position
      delete this->last_mouse_pos_;
      this->last_mouse_pos_ = new sf::Vector2f(c.x, c.y);
   } else if (c.button == MouseButtonCommand::RIGHT) {
      main_layer->drag(c, delta);
   }
}

float BuilderScene::get_scale() {
   Layer* main_layer = this->viewport_->get("main");
   if (main_layer) {
      return main_layer->get_scale();
   }

   Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
   return 1.0;
}

void BuilderScene::set_scale(float factor) {
   Layer* main_layer = this->viewport_->get("main");
   if (main_layer) {
      main_layer->set_scale(factor);
   } else {
      Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
   }
}

void BuilderScene::click(MouseButtonCommand& c) {
   if (c.button == MouseButtonCommand::LEFT) {
      if (c.state == MouseButtonCommand::PRESSED) {
         if (this->click_release_pos_) {
            return;
         }

         delete this->click_press_pos_;
         this->click_press_pos_ = new sf::Vector2f(c.x, c.y);
      } else if (c.state == MouseButtonCommand::RELEASED) {
         if (this->selection_rectangle_) {
            // remove selection rectangle from entities, layer, and delete it
            EntityList::const_iterator it;
            for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
               if (*it == this->selection_rectangle_) {
                  this->entities_.erase(it);
                  break;
               }
            }

            this->viewport_->get("main")->remove(this->selection_rectangle_);
            delete this->selection_rectangle_;
            this->selection_rectangle_ = nullptr;
         }

         delete this->click_press_pos_;
         this->click_press_pos_ = nullptr;

         delete this->last_mouse_pos_;
         this->last_mouse_pos_ = nullptr;
         return;
      }
   } else if (c.button == MouseButtonCommand::RIGHT) {
   }

   //sf::Vector2f world_coord;

   //try {
   //   world_coord = this->viewports_.at("main")->get_world_coord(sf::Vector2i(c.x, c.y));
   //}
   //catch (const std::out_of_range& e) {
   //   Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
   //   return; // not sure if this is necessary...
   //}

   //// on right click, begin select behavior
   //if (c.button == MouseButtonCommand::MOUSE_BUTTON::LEFT && c.state == MouseButtonCommand::STATE::PRESSED) {
   //   this->click_press_pos = new sf::Vector2f(world_coord);
   //}

   //// on left click, select a map tile
   //if (c.button == MouseButtonCommand::MOUSE_BUTTON::LEFT && c.state == MouseButtonCommand::STATE::RELEASED) {
   //   Map::tiles_t tiles;
   //   bool is_one_tile_selected = false;

   //   Service::get_logger().msg("BuilderScene", Logger::INFO, "world_coord: (" + std::to_string(world_coord.x) + ", " + std::to_string(world_coord.y) + ")");

   //   this->click_release_pos = new sf::Vector2f(world_coord);

   //   is_one_tile_selected = (std::abs(this->click_press_pos->x - this->click_release_pos->x) < 10) &&
   //      (std::abs(this->click_press_pos->y - this->click_release_pos->y) < 10);

   //   Service::get_logger().msg("BuilderScene", Logger::INFO, "is_one_tile_selected? " + std::to_string(is_one_tile_selected));

   //   // map click and release positions to origin and end
   //   sf::Vector2f* click_origin_pos = new sf::Vector2f(0, 0);
   //   sf::Vector2f* click_end_pos = new sf::Vector2f(0, 0);
   //
   //   click_origin_pos->x = std::min(this->click_press_pos->x, this->click_release_pos->x);
   //   click_origin_pos->y = std::min(this->click_press_pos->y, this->click_release_pos->y);

   //   click_end_pos->x = std::max(this->click_press_pos->x, this->click_release_pos->x);
   //   click_end_pos->y = std::max(this->click_press_pos->y, this->click_release_pos->y);

   //   // round press and release click positions to tiles
   //   click_origin_pos->x = Settings::Instance()->TILE_WIDTH * ((int)(click_origin_pos->x / Settings::Instance()->TILE_WIDTH) - (click_origin_pos->x < 0 ? 1 : 0));
   //   click_origin_pos->y = Settings::Instance()->TILE_HEIGHT * ((int)(click_origin_pos->y / Settings::Instance()->TILE_HEIGHT) - (click_origin_pos->y < 0 ? 1 : 0));

   //   click_end_pos->x = Settings::Instance()->TILE_WIDTH * ((int)(click_end_pos->x / Settings::Instance()->TILE_WIDTH) + ((int)click_end_pos->x % (int)Settings::Instance()->TILE_WIDTH ? 1 : 0) - (click_end_pos->x < 0 ? 1 : 0));
   //   click_end_pos->y = Settings::Instance()->TILE_HEIGHT * ((int)(click_end_pos->y / Settings::Instance()->TILE_HEIGHT) + ((int)click_end_pos->y % (int)Settings::Instance()->TILE_HEIGHT ? 1 : 0) - (click_end_pos->y < 0 ? 1 : 0));

   //   sf::Vector2f selected_size(*click_end_pos - *click_origin_pos);
   //   selected_size.x = std::max(selected_size.x, Settings::Instance()->TILE_WIDTH);
   //   selected_size.y = std::max(selected_size.y, Settings::Instance()->TILE_HEIGHT);

   //   sf::FloatRect selected(*click_origin_pos, selected_size);
   //   tiles = this->map->intersects(selected);

   //   delete this->click_press_pos;
   //   delete this->click_release_pos;

   //   delete click_origin_pos;
   //   delete click_end_pos;

   //   this->click_press_pos = nullptr;
   //   this->click_release_pos = nullptr;

   //   if (this->selected_tile) {
   //      bool selected_is_one_tile = false;
   //      PhysicsPart* selected_physics = dynamic_cast<PhysicsPart*>(this->selected_tile->get("physics"));
   //      
   //      if (!selected_physics) {
   //         delete this->selected_tile;
   //         this->selected_tile = nullptr;

   //         Service::get_logger().msg("BuilderScene", Logger::WARNING, "Tile cursor does not have physics component.");
   //         return;
   //      }

   //      selected_is_one_tile = ((int)selected_physics->get_size().x == (int)Settings::Instance()->TILE_WIDTH);
   //      Service::get_logger().msg("BuilderScene", Logger::INFO, "selected_is_one_tile? " + std::to_string(selected_is_one_tile));

   //      // TODO: wow this is terrible... "selected_is_one_tile" is about the existing cursor and "is_one_tile_selected" is about the mouse drag selection
   //      if (!is_one_tile_selected || 
   //         (!selected_physics->intersects(world_coord) &&
   //          ((selected_is_one_tile && is_one_tile_selected) ||
   //          (selected_is_one_tile && !is_one_tile_selected) ||
   //          (!selected_is_one_tile && !is_one_tile_selected)))) {
   //         delete this->selected_tile;
   //         this->selected_tile = TileFactory::inst()->create_tile_cursor(sf::Vector2f(selected.left, selected.top), sf::Vector2f(selected.width, selected.height), tiles);
   //      } else {
   //         delete this->selected_tile;
   //         this->selected_tile = nullptr;
   //      }
   //   } else {
   //      this->selected_tile = TileFactory::inst()->create_tile_cursor(sf::Vector2f(selected.left, selected.top), sf::Vector2f(selected.width, selected.height), tiles);
   //   }
   //}
}
