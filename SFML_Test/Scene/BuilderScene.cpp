#include "BuilderScene.h"

#include "Game.h"
#include "Layer.h"

#include "Graphic.h"

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
, last_mouse_pos_(nullptr)
, click_press_pos_(nullptr)
, click_release_pos_(nullptr)

, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
{
}

BuilderScene::~BuilderScene() {
   delete this->map_;

   delete this->tile_cursor_;

   delete this->last_mouse_pos_;

   delete this->click_press_pos_;
   delete this->click_release_pos_;

   //std::vector<sf::RectangleShape*>::const_iterator it;
   //for (it = this->grid.begin(); it != this->grid.end(); ++it) {
   //   delete *it;
   //}
   //this->grid.clear();
}

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg("BuilderScene", Logger::INFO, "Entering builder start menu state.");
   
   // create viewport(s)
   this->viewport_ = new Viewport(sf::Vector2f(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT));

   // fixed layer above map and sprites
   this->viewport_->add("overlay");
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
   this->viewport_->layer("main")->add(this->map_); // add map to draw list

   delete serializer;
   delete map_builder;

   // initialize entities
   this->mouse_ = UtilFactory::inst()->create_mouse();
   this->entities_.push_back(this->mouse_);
   this->viewport_->layer("hud")->add(this->mouse_);

   // hook up mouse controller to user input
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->mouse_->get("control")));

   // let our mouse controller manipulate this scene
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(this);
   
   // create fixed hud items
   Entity* t = TextFactory::inst()->create_text_entity("SFML_Test", "retro");
   this->viewport_->layer("hud")->add(t);
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("r: reset pan position", "retro", sf::Vector2f(0, 15));
   this->viewport_->layer("hud")->add(t);
   this->entities_.push_back(t);
   
   t = TextFactory::inst()->create_text_entity("right click: click and drag to pan", "retro", sf::Vector2f(0, 30));
   this->viewport_->layer("hud")->add(t);
   this->entities_.push_back(t);

   Shape* origin_dot = new Shape(new sf::RectangleShape());
   origin_dot->set_size(3, 3);
   origin_dot->set_fill_color(sf::Color::Yellow);
   Entity* origin_dot_entity = UtilFactory::inst()->create_graphic(origin_dot, origin_dot->get_global_bounds());

   this->entities_.push_back(origin_dot_entity);
   this->viewport_->layer("main")->add(origin_dot_entity);

   Shape* center_dot_graphic = new Shape(new sf::RectangleShape());
   center_dot_graphic->set_size(3, 3);
   center_dot_graphic->set_fill_color(sf::Color(255, 104, 2));
   center_dot_graphic->set_position(Settings::Instance()->SCREEN_WIDTH / 2, Settings::Instance()->SCREEN_HEIGHT / 2);
   this->center_dot_ = UtilFactory::inst()->create_graphic(center_dot_graphic, center_dot_graphic->get_global_bounds());

   this->entities_.push_back(this->center_dot_);
   this->viewport_->layer("hud")->add(this->center_dot_);

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
      Shape* sr = nullptr;

      if (!this->selection_rectangle_) {
         sr = new Shape(new sf::RectangleShape());
         sr->set_fill_color(sf::Color(66, 108, 167, 175));
         sr->set_outline_color(sf::Color(124, 160, 210, 192));
         sr->set_outline_thickness(1.0);

         this->selection_rectangle_ = UtilFactory::inst()->create_graphic(sr, sr->get_global_bounds());

         this->entities_.push_back(this->selection_rectangle_);
         this->viewport_->layer("overlay")->add(this->selection_rectangle_);
      }

      sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(*this->click_press_pos_, *this->last_mouse_pos_);

      this->selection_rectangle_->set_position(new_rect->left, new_rect->top);
      this->selection_rectangle_->set_size(new_rect->width, new_rect->height);

      delete new_rect;
      new_rect = nullptr;

      delete this->last_mouse_pos_;
      this->last_mouse_pos_ = nullptr;
   }

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
   Map::TileGrid::const_iterator it;

   switch (c.event.code) {
   case sf::Keyboard::Key::R:
      this->viewport_->reset();
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
      // toggle debug text on all tiles
      for (it = this->map_->get_tiles().begin(); it != this->map_->get_tiles().end(); ++it) {
         GraphicsPart* tile_graphics = dynamic_cast<GraphicsPart*>(it->second->get("graphics"));

         if (tile_graphics) {
            tile_graphics->set_show_debug_text(!tile_graphics->get_show_debug_text());
         }
      }
   break;
   default:
      // do nothing
   break;
   }
}

void BuilderScene::process(Game& game, WindowResizeCommand& c) {
   sf::Vector2f new_size(c.width, c.height);
   sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

   // update viewport
   this->viewport_->resize(new_size);
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
   Layer* main_layer = this->viewport_->layer("main");
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
   Layer* main_layer = this->viewport_->layer("main");
   if (main_layer) {
      return main_layer->get_scale();
   }

   Service::get_logger().msg("BuilderScene", Logger::WARNING, "Main viewport cannot be found...");
   return 1.0;
}

void BuilderScene::set_scale(float factor) {
   Layer* main_layer = this->viewport_->layer("main");
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
            // this shouldn't happen
            return;
         }

         delete this->click_press_pos_;
         this->click_press_pos_ = new sf::Vector2f(c.x, c.y);
      } else if (c.state == MouseButtonCommand::RELEASED) {
         delete this->click_release_pos_;
         this->click_release_pos_ = new sf::Vector2f(c.x, c.y);

         if (!this->click_press_pos_) {
            Service::get_logger().msg("BuilderScene", Logger::WARNING, "click press position missing");
            return;
         }

         bool is_drag_gesture = false;
         if (this->selection_rectangle_) {
            PhysicsPart* sr = dynamic_cast<PhysicsPart*>(this->selection_rectangle_->get("physics"));
            sf::Vector2f sr_size = sr->get_size();

            is_drag_gesture = (sr_size.x >= Settings::Instance()->TILE_WIDTH / 3.f) && (sr_size.y >= Settings::Instance()->TILE_HEIGHT / 3.f);
         }

         // where should I put this code?
         *this->click_press_pos_ -= this->viewport_->layer("main")->get_pan_delta();
         *this->click_release_pos_ -= this->viewport_->layer("main")->get_pan_delta();

         if (!this->tile_cursor_) {
            // create a tile cursor
            Map::TileList cursor_tiles;

            delete this->tile_cursor_;
            this->tile_cursor_ = TileFactory::inst()->create_tile_cursor(*this->click_press_pos_, *this->click_release_pos_, cursor_tiles);
            this->round_to_nearest_tile(*this->click_press_pos_, *this->click_release_pos_);

            this->viewport_->layer("main")->add(this->tile_cursor_);
         } else {
            PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(this->tile_cursor_->get("physics"));

            if (is_drag_gesture || (!is_drag_gesture && !tc_physics->intersects(*this->click_press_pos_))) {
               this->round_to_nearest_tile(*this->click_press_pos_, *this->click_release_pos_);
            } else {
               // deselect
               this->viewport_->layer("main")->remove(this->tile_cursor_);
               delete this->tile_cursor_;
               this->tile_cursor_ = nullptr;
            }
         }

         // remove selection rectangle from entities, layer, and delete it
         if (this->selection_rectangle_) {
            EntityList::const_iterator it;
            for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
               if (*it == this->selection_rectangle_) {
                  this->entities_.erase(it);
                  break;
               }
            }

            this->viewport_->layer("overlay")->remove(this->selection_rectangle_);
            delete this->selection_rectangle_;
            this->selection_rectangle_ = nullptr;
         }

         delete this->click_press_pos_;
         this->click_press_pos_ = nullptr;

         delete this->click_release_pos_;
         this->click_release_pos_ = nullptr;
      }
   }
}

void BuilderScene::round_to_nearest_tile(sf::Vector2f& one, sf::Vector2f& two) {
   if (!this->tile_cursor_) {
      return;
   }

   PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(this->tile_cursor_->get("physics"));
   GraphicsPart* tc_graphics = dynamic_cast<GraphicsPart*>(this->tile_cursor_->get("graphics"));
   Graphic* tc_rect = tc_graphics->get(0);

   sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(one, two);

   float tile_width = Settings::Instance()->TILE_WIDTH;
   float tile_height = Settings::Instance()->TILE_HEIGHT;

   // round to nearest tile
   sf::Vector2f end_point(new_rect->left + new_rect->width, new_rect->top + new_rect->height);
   end_point.x = tile_width * std::round(end_point.x / tile_width);
   end_point.y = tile_height * std::round(end_point.y / tile_height);

   new_rect->left = tile_width * std::floor(new_rect->left / tile_width);
   new_rect->top = tile_height * std::floor(new_rect->top / tile_height);

   new_rect->width = end_point.x - new_rect->left;
   new_rect->height = end_point.y - new_rect->top;

   // ensure minimum size
   new_rect->width = std::max(new_rect->width, tile_width);
   new_rect->height = std::max(new_rect->height, tile_height);

   sf::Vector2f final_origin = sf::Vector2f(new_rect->left, new_rect->top);
   sf::Vector2f final_end = sf::Vector2f(new_rect->left + new_rect->width, new_rect->top + new_rect->height);

   sf::Vector2f final_size = final_end - final_origin;

   // update tile cursor entity
   tc_physics->set_position(final_origin);
   tc_physics->set_size(final_size);
   
   tc_rect->set_position(final_origin);
   tc_rect->set_size(final_size);

   delete new_rect;
}
