#include "BuilderScene.h"

#include "Game.h"
#include "Layer.h"

#include "Graphic.h"

#include "Entity.h"
#include "DebugPart.h"
#include "GraphicsPart.h"
#include "PhysicsPart.h"
#include "ReferencePart.h"
#include "MouseControlPart.h"

#include "UtilFactory.h"
#include "TileFactory.h"
#include "TextFactory.h"
#include "TextSerializer.h"
#include "FlatMapBuilder.h"
#include "Grid.h"

BuilderScene::BuilderScene()
: Scene("BuilderScene")
, map_(nullptr)
, mouse_(nullptr)
, center_dot_(nullptr)
, selection_rectangle_(nullptr)
, tile_cursor_(nullptr)
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
, show_debug_info_(false)
{
}

BuilderScene::~BuilderScene() {
   delete this->map_;
   delete this->mouse_;

   this->remove_tile_cursor();
}

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering builder start menu state.");
   
   // create viewport(s)
   this->viewport_ = new Viewport(sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height()));

   // fixed layer above map and sprites
   this->viewport_->add("grid");
   this->viewport_->add("overlay");
   this->viewport_->add("hud");

   this->viewport_->layer("grid")->hide(); // hide grid display layer by default

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
   // TODO: need to change this so that individual tiles can specify which layer they are on
   this->map_->layer(this->viewport_->layer("main"));

   this->map_->grid()->layer(this->viewport_->layer("grid"));
   
   delete serializer;
   delete map_builder;

   // initialize entities
   this->mouse_ = UtilFactory::inst()->create_mouse(this->viewport_->layer("hud"));

   // hook up mouse controller to user input
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->mouse_->get("control")));

   // let our mouse controller manipulate this scene
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(this);
   
   // create fixed hud items
   Entity* t = TextFactory::inst()->create_text_entity("SFML_Test", "retro", this->viewport_->layer("hud"));
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("r: reset pan position", "retro", this->viewport_->layer("hud"), sf::Vector2f(0, 15));
   this->entities_.push_back(t);
   
   t = TextFactory::inst()->create_text_entity("right click: click and drag to pan", "retro", this->viewport_->layer("hud"), sf::Vector2f(0, 30));
   this->entities_.push_back(t);

   Shape* center_dot_graphic = new Shape(new sf::RectangleShape());
   center_dot_graphic->set_size(3, 3);
   center_dot_graphic->set_fill_color(sf::Color(255, 104, 2));
   center_dot_graphic->set_position(Settings::Instance()->cur_width() / 2, Settings::Instance()->cur_height() / 2);
   center_dot_graphic->layer(this->viewport_->layer("hud"));

   this->center_dot_ = UtilFactory::inst()->create_graphic(
      center_dot_graphic,
      center_dot_graphic->get_global_bounds(),
      this->show_debug_info_
   );
   this->entities_.push_back(this->center_dot_);

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", "retro", this->viewport_->layer("hud"));
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);
   this->entities_.push_back(this->fps_display_);
}

void BuilderScene::exit(Game& game) {
}

void BuilderScene::draw(Graphics& graphics) {
   Scene::draw(graphics);
}

void BuilderScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);

   this->map_->update(game, this);
   this->mouse_->update(game, this);
   
   // calculate and show FPS
   if (!this->frame_count) {
      this->update_fps();
   }
   this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
}

void BuilderScene::process(Game& game, CloseCommand& c) {
   game.unload_scene();
}

void BuilderScene::process(Game& game, KeyPressCommand& c) {
   switch (c.event.code) {
   case sf::Keyboard::Key::R:
      this->viewport_->reset();

      // reset grid position too
      this->map_->grid()->set_position(sf::Vector2f(0, 0));
   break;
   case sf::Keyboard::Key::F:
      //if (this->selected_tile) {
      //   // TODO: refactor when infrastructure for this is up
      //   Service::get_logger().msg(this->id_, Logger::INFO, "Adding new tiles.");

      //   PhysicsPart* selected_physics = dynamic_cast<PhysicsPart*>(this->selected_tile->get("physics"));
      //   if (!selected_physics) {
      //      Service::get_logger().msg(this->id_, Logger::WARNING, "In process KeyPressCommand (F) -> selected tile cursor has no physics component.");
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
      //         Service::get_logger().msg(this->id_, Logger::INFO, "checking tile " + std::to_string(next_x) + ", " + std::to_string(next_y));

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
      //            Service::get_logger().msg(this->id_, Logger::INFO, "poop");
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
   case sf::Keyboard::Key::O:
      this->toggle_debug_info();      
   break;
   case sf::Keyboard::Key::G:
      // toggle map grid visibility
      if (this->viewport_->layer("grid")->visible()) {
         this->viewport_->layer("grid")->hide();
      } else {
         this->viewport_->layer("grid")->show();
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
   this->center_dot_->set_position(new_center);

   // reposition fps display
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);

   // update grid
   sf::Vector2f inverse_pan_delta = this->viewport_->layer("main")->get_pan_delta();
   inverse_pan_delta.x *= -1;
   inverse_pan_delta.y *= -1;

   this->map_->grid()->set_position(inverse_pan_delta);
}

void BuilderScene::process(Game& game, MouseButtonCommand& c) {}
void BuilderScene::process(Game& game, MouseMoveCommand& c) {}
void BuilderScene::process(Game& game, MouseWheelCommand& c) {}

void BuilderScene::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   Layer* main_layer = this->viewport_->layer("main");
   if (!main_layer) {
      Service::get_logger().msg(this->id_, Logger::WARNING, "Main viewport cannot be found...");
      return;
   }

   Layer* grid_layer = this->viewport_->layer("grid");
   if (!grid_layer) {
      Service::get_logger().msg(this->id_, Logger::WARNING, "Grid viewport cannot be found...");
      return;
   }

   if (c.button == MouseButtonCommand::LEFT) {
      sf::Vector2f mouse_pos(c.x, c.y);
      this->update_selection_rect(this->click_press_pos_, mouse_pos);
   } else if (c.button == MouseButtonCommand::RIGHT) {
      main_layer->drag(c, delta);
      grid_layer->drag(c, delta);
      
      this->map_->grid()->move(delta);
   }
}

float BuilderScene::get_scale() {
   Layer* main_layer = this->viewport_->layer("main");
   if (main_layer) {
      return main_layer->get_scale();
   }

   Service::get_logger().msg(this->id_, Logger::WARNING, "Main viewport cannot be found...");
   return 1.0;
}

void BuilderScene::set_scale(float factor) {
   Layer* main_layer = this->viewport_->layer("main");
   if (main_layer) {
      main_layer->set_scale(factor);
   } else {
      Service::get_logger().msg(this->id_, Logger::WARNING, "Main viewport cannot be found...");
   }

   Layer* grid_layer = this->viewport_->layer("grid");
   if (grid_layer) {
      grid_layer->set_scale(factor);
   } else {
      Service::get_logger().msg(this->id_, Logger::WARNING, "Grid viewport cannot be found...");
   }
}

void BuilderScene::click(MouseButtonCommand& c) {
   if (c.button == MouseButtonCommand::LEFT) {
      if (c.state == MouseButtonCommand::PRESSED) {
         this->click_press_pos_.x = c.x;
         this->click_press_pos_.y = c.y;

         this->register_selection_rect();
         this->update_selection_rect(this->click_press_pos_, this->click_press_pos_);
      } else if (c.state == MouseButtonCommand::RELEASED) {
         this->deregister_selection_rect();

         sf::Vector2f click_release_pos(c.x, c.y);
         this->update_tile_cursor(this->click_press_pos_, click_release_pos);
      }
   }
}

void BuilderScene::update_fps() {
   this->last_frame_time = (((float)this->frame_measurement_interval / this->clock.getElapsedTime().asSeconds()) * Settings::Instance()->FRAMERATE_SMOOTHING)
                           + (this->last_frame_time * (1.0 - Settings::Instance()->FRAMERATE_SMOOTHING));
   this->clock.restart();

   // update fps entity
   GraphicsPart* fps_graphic = dynamic_cast<GraphicsPart*>(fps_display_->get("graphics"));
   if (fps_graphic) {
      fps_graphic->get(0)->set_string("FPS: " + std::to_string(this->last_frame_time));
   }
}

void BuilderScene::toggle_debug_info() {
   EntityList::const_iterator e_it;
   Map::TileList::const_iterator it;

   this->show_debug_info_ = !this->show_debug_info_;

   if (this->show_debug_info_) {
      // turned debug info on, add debug components to all entities
      for (it = this->map_->get_tiles().begin(); it != this->map_->get_tiles().end(); ++it) {
         (*it)->add(new DebugPart());
      }

      for (e_it = this->entities_.begin(); e_it != this->entities_.end(); ++e_it) {
         (*e_it)->add(new DebugPart());
      }
   } else {
      // turned debug info off, remove debug components from all entities
      for (it = this->map_->get_tiles().begin(); it != this->map_->get_tiles().end(); ++it) {
         (*it)->remove("debug");
      }

      for (e_it = this->entities_.begin(); e_it != this->entities_.end(); ++e_it) {
         (*e_it)->remove("debug");
      }
   }
}

void BuilderScene::register_selection_rect() {
   if (!this->selection_rectangle_) {
      this->selection_rectangle_ = TileFactory::inst()->create_selection_rectangle(
         nullptr,
         this->viewport_->layer("overlay"),
         this->show_debug_info_
      );

      this->entities_.push_back(this->selection_rectangle_);
   } else {
      this->selection_rectangle_->layer(this->viewport_->layer("overlay"));
   }
}

void BuilderScene::deregister_selection_rect() {
   if (!this->selection_rectangle_) {
      return;
   }
   
   // keep it in registered scene entities list, but remove it from screen
   this->selection_rectangle_->layer(nullptr);
}

void BuilderScene::update_selection_rect(sf::Vector2f& origin_click, sf::Vector2f& mouse_pos) {
   if (!this->selection_rectangle_) {
      this->selection_rectangle_ = TileFactory::inst()->create_selection_rectangle(
         nullptr,
         this->viewport_->layer("overlay"),
         this->show_debug_info_
      );

      this->entities_.push_back(this->selection_rectangle_);
   } else {
      this->selection_rectangle_->layer(this->viewport_->layer("overlay"));
   }

   sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(origin_click, mouse_pos);

   this->selection_rectangle_->set_position(new_rect->left, new_rect->top);
   this->selection_rectangle_->set_size(new_rect->width, new_rect->height);

   delete new_rect;
}

void BuilderScene::update_tile_cursor(sf::Vector2f& one, sf::Vector2f& two) {
   // compensate for main viewport layer zoom
   sf::Vector2f offset_one = this->viewport_->layer("main")->get_scale() * (one - this->viewport_->layer("hud")->get_center()) + this->viewport_->layer("hud")->get_center();
   sf::Vector2f offset_two = this->viewport_->layer("main")->get_scale() * (two - this->viewport_->layer("hud")->get_center()) + this->viewport_->layer("hud")->get_center();
   
   // compensate for the panning of main viewport layer
   sf::Vector2f pan_delta = this->viewport_->layer("main")->get_pan_delta();
   offset_one -= pan_delta;
   offset_two -= pan_delta;

   sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(offset_one, offset_two);

   bool is_drag_gesture = (new_rect->width >= this->map_->grid()->tile_width() / 3.f ||
                           new_rect->height >= this->map_->grid()->tile_height() / 3.f);

   if (!is_drag_gesture && this->tile_cursor_ && this->tile_cursor_->intersects(offset_one)) {
      this->remove_tile_cursor();
      delete new_rect;
      return;
   }

   if (!this->tile_cursor_) {
      Map::TileList cursor_tiles;
      this->tile_cursor_ = TileFactory::inst()->create_tile_cursor(
         one,
         two,
         cursor_tiles,
         this->viewport_->layer("main"),
         this->show_debug_info_
      );

      this->entities_.push_back(this->tile_cursor_);
   }

   // round to nearest map grid point
   sf::Vector2f start_point(new_rect->left, new_rect->top);
   sf::Vector2f end_point(new_rect->left + new_rect->width, new_rect->top + new_rect->height);

   start_point = this->map_->grid()->floor(start_point);
   end_point = this->map_->grid()->ceil(end_point);

   // update new_rect
   new_rect->left = start_point.x;
   new_rect->top = start_point.y;

   new_rect->width = std::max(end_point.x - new_rect->left, (float)this->map_->grid()->tile_width());
   new_rect->height = std::max(end_point.y - new_rect->top, (float)this->map_->grid()->tile_height());

   this->tile_cursor_->set_position(new_rect->left, new_rect->top);
   this->tile_cursor_->set_size(new_rect->width, new_rect->height);

   delete new_rect;
}

void BuilderScene::remove_tile_cursor() {
   EntityList::const_iterator it;
   for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
      if (*it == this->tile_cursor_) {
         this->entities_.erase(it);
         break;
      }
   }

   delete this->tile_cursor_;
   this->tile_cursor_ = nullptr;
}
