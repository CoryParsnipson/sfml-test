#include "BuilderScene.h"
#include "TestUIScene.h"

#include "Game.h"

#include "RenderSurface.h"
#include "Texture.h"
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
, backdrop_(sf::TrianglesStrip, 4)
, map_filename_("flat_map_test.txt")
, mouse_(nullptr)
, center_dot_(nullptr)
, selection_rectangle_(nullptr)
, tile_cursor_(nullptr)
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
, show_debug_info_(false)
{
   // load textures
   TextureManager::inst()->create_texture("tile_solid", "flatmap_test_texture.png", sf::IntRect(0, 0, 40, 40));
   TextureManager::inst()->create_texture("tile_clear", "flatmap_test_texture.png", sf::IntRect(40, 0, 40, 40));
   TextureManager::inst()->print();

   // build the map
   this->serializer_ = new TextSerializer();
   this->serializer_->open_infile(this->map_filename_);

   MapBuilder* map_builder = new FlatMapBuilder();
   map_builder->set_serializer(this->serializer_);
   map_builder->build();

   this->map_ = map_builder->get_map();

   this->backdrop_[0].position = sf::Vector2f(0, 0);
   this->backdrop_[1].position = sf::Vector2f(0, Settings::Instance()->cur_height());
   this->backdrop_[2].position = sf::Vector2f(Settings::Instance()->cur_width(), 0);
   this->backdrop_[3].position = sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height());

   this->backdrop_[0].color = sf::Color(50, 50, 50, 255);
   this->backdrop_[1].color = sf::Color(25, 25, 25, 255);
   this->backdrop_[2].color = sf::Color(50, 50, 50, 255);
   this->backdrop_[3].color = sf::Color(25, 25, 25, 255);

   delete map_builder;

   // initialize entities
   this->mouse_ = UtilFactory::inst()->create_mouse(this->mouse_layer);
   this->entities_.push_back(this->mouse_);

   // let our mouse controller manipulate this scene
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(this);

   // create fixed hud items
   Entity* t = TextFactory::inst()->create_text_entity("SFML_Test", "retro");
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("r: reset pan position", "retro", sf::Vector2f(0, 15));
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("g: toggle grid visibility", "retro", sf::Vector2f(0, 30));
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("o: toggle entity hitboxes", "retro", sf::Vector2f(0, 45));
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("1: add green tiles at selection", "retro", sf::Vector2f(0, 60));
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("2: add blue tiles at selection", "retro", sf::Vector2f(0, 75));
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("del: remove tiles at selection", "retro", sf::Vector2f(0, 90));
   this->entities_.push_back(t);

   t = TextFactory::inst()->create_text_entity("right click: click and drag to pan", "retro", sf::Vector2f(0, 105));
   this->entities_.push_back(t);

   Shape* center_dot_graphic = new Shape(new sf::RectangleShape());
   center_dot_graphic->set_size(3, 3);
   center_dot_graphic->set_fill_color(sf::Color(255, 104, 2));
   center_dot_graphic->set_position(Settings::Instance()->cur_width() / 2, Settings::Instance()->cur_height() / 2);

   this->center_dot_ = UtilFactory::inst()->create_graphic(
      center_dot_graphic,
      center_dot_graphic->get_global_bounds(),
      this->show_debug_info_
   );
   this->entities_.push_back(this->center_dot_);

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", "retro");
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);
   this->entities_.push_back(this->fps_display_);
}

BuilderScene::~BuilderScene() {
   delete this->map_;
   delete this->serializer_;

   this->remove_tile_cursor();
}

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering builder start menu state.");

   // hook up mouse controller to user input
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->mouse_->get("control")));
}

void BuilderScene::exit(Game& game) {
   // unregister mouse control from input listener
   Service::get_input().unregisterInputListener(dynamic_cast<InputListener*>(this->mouse_->get("control")));
}

void BuilderScene::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(this->backdrop_, render_states);

   Scene::draw(surface, render_states);
   this->map_->draw(surface, render_states);
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
      this->camera_->reset_pan();
      this->camera_->reset_zoom();

      // reset grid too
      this->map_->grid()->set_position(sf::Vector2f(0, 0));
      this->map_->grid()->set_scale(1.0);
   break;
   case sf::Keyboard::Key::Num1:
   case sf::Keyboard::Key::Numpad1:
      this->set_tiles(TextureManager::inst()->get_texture("tile_solid"));
   break;
   case sf::Keyboard::Key::Num2:
   case sf::Keyboard::Key::Numpad2:
      this->set_tiles(TextureManager::inst()->get_texture("tile_clear"));
   break;
   case sf::Keyboard::Key::O:
      this->toggle_debug_info();
   break;
   case sf::Keyboard::Key::G:
      // toggle map grid visibility
      // TODO: fix this
      // if (this->viewport_->layer("grid")->visible()) {
      //    this->viewport_->layer("grid")->hide();
      // } else {
      //    this->viewport_->layer("grid")->show();
      // }
   break;
   case sf::Keyboard::Key::Delete:
   case sf::Keyboard::Key::BackSpace:
      this->remove_tiles();
   break;
   case sf::Keyboard::Key::S:
      Service::get_logger().msg(this->id_, Logger::INFO, "Writing map to file '" + this->map_filename_ + "'");
      this->map_->serialize(*this->serializer_);
   break;
   case sf::Keyboard::Key::Escape:
      // load super secret test ui scene
      game.switch_scene(new TestUIScene());
   break;
   default:
      // do nothing
   break;
   }
}

void BuilderScene::process(Game& game, WindowResizeCommand& c) {
   Scene::process(game, c);

   sf::Vector2f new_size(c.width, c.height);
   sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

   // reposition center dot
   this->center_dot_->set_position(new_center);

   // reposition fps display
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);

   // update grid
   sf::Vector2f inverse_pan_delta = this->camera_->get_pan_delta();
   inverse_pan_delta.x *= -1;
   inverse_pan_delta.y *= -1;

   this->map_->grid()->set_position(inverse_pan_delta);

   // update backsplash
   this->backdrop_[0].position = sf::Vector2f(0, 0);
   this->backdrop_[1].position = sf::Vector2f(0, Settings::Instance()->cur_height());
   this->backdrop_[2].position = sf::Vector2f(Settings::Instance()->cur_width(), 0);
   this->backdrop_[3].position = sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height());
}

void BuilderScene::process(Game& game, MouseButtonCommand& c) {}
void BuilderScene::process(Game& game, MouseMoveCommand& c) {}
void BuilderScene::process(Game& game, MouseWheelCommand& c) {}

void BuilderScene::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   delta = -1.f * delta;

   if (c.button == MouseButtonCommand::LEFT) {
      sf::Vector2f mouse_pos(c.x, c.y);
      this->update_selection_rect(this->click_press_pos_, mouse_pos);
   } else if (c.button == MouseButtonCommand::RIGHT) {
      // TODO: drag everything in the tile map layer entity group
      this->map_->grid()->move(delta);
   }
}

float BuilderScene::get_scale() {
   return this->camera_->get_scale();
}

void BuilderScene::set_scale(float factor) {
   this->camera_->set_scale(factor);
   this->map_->grid()->set_scale(factor);
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
   EntityList::const_iterator it;

   this->show_debug_info_ = !this->show_debug_info_;

   if (this->show_debug_info_) {
      // turned debug info on, add debug components to all entities
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         (*it)->add(new DebugPart());
      }
   } else {
      // turned debug info off, remove debug components from all entities
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         (*it)->remove("debug");
      }
   }
}

void BuilderScene::register_selection_rect() {
   if (!this->selection_rectangle_) {
      this->selection_rectangle_ = TileFactory::inst()->create_selection_rectangle(
         nullptr,
         1,
         this->show_debug_info_
      );

      this->entities_.push_back(this->selection_rectangle_);
   } else {
      //this->selection_rectangle_->layer(1);
   }
}

void BuilderScene::deregister_selection_rect() {
   if (!this->selection_rectangle_) {
      return;
   }

   // keep it in registered scene entities list, but remove it from screen
   //this->selection_rectangle_->layer(nullptr);
}

void BuilderScene::update_selection_rect(sf::Vector2f& origin_click, sf::Vector2f& mouse_pos) {
   if (!this->selection_rectangle_) {
      this->selection_rectangle_ = TileFactory::inst()->create_selection_rectangle(
         nullptr,
         1,
         this->show_debug_info_
      );

      this->entities_.push_back(this->selection_rectangle_);
   } else {
      //this->selection_rectangle_->layer(1);
   }

   sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(origin_click, mouse_pos);

   this->selection_rectangle_->set_position(new_rect->left, new_rect->top);
   this->selection_rectangle_->set_size(new_rect->width, new_rect->height);

   delete new_rect;
}

void BuilderScene::update_tile_cursor(sf::Vector2f& one, sf::Vector2f& two) {
   // compensate for main viewport layer zoom
   // TODO: fix this
   //sf::Vector2f offset_one = this->viewport_->layer("main")->get_scale() * (one - this->viewport_->layer("hud")->get_center()) + this->viewport_->layer("hud")->get_center();
   //sf::Vector2f offset_two = this->viewport_->layer("main")->get_scale() * (two - this->viewport_->layer("hud")->get_center()) + this->viewport_->layer("hud")->get_center();

   // compensate for the panning of main viewport layer
   // sf::Vector2f pan_delta = this->camera_->get_pan_delta();
   // offset_one -= pan_delta;
   // offset_two -= pan_delta;
   //
   // sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(offset_one, offset_two);
   //
   // bool is_drag_gesture = (new_rect->width >= this->map_->grid()->tile_width() / 3.f ||
   //                         new_rect->height >= this->map_->grid()->tile_height() / 3.f);
   //
   // if (!is_drag_gesture && this->tile_cursor_ && this->tile_cursor_->intersects(offset_one)) {
   //    this->remove_tile_cursor();
   //    delete new_rect;
   //    return;
   // }
   //
   // if (!this->tile_cursor_) {
   //    this->tile_cursor_ = TileFactory::inst()->create_tile_cursor(
   //       one,
   //       two,
   //       this->viewport_->layer("main"),
   //       this->show_debug_info_
   //    );
   //
   //    this->entities_.push_back(this->tile_cursor_);
   // }
   //
   // // round to nearest map grid point
   // sf::Vector2f start_point(new_rect->left, new_rect->top);
   // sf::Vector2f end_point(new_rect->left + new_rect->width, new_rect->top + new_rect->height);
   //
   // start_point = this->map_->grid()->floor(start_point);
   // end_point = this->map_->grid()->ceil(end_point);
   //
   // // update new_rect
   // new_rect->left = start_point.x;
   // new_rect->top = start_point.y;
   //
   // new_rect->width = std::max(end_point.x - new_rect->left, (float)this->map_->grid()->tile_width());
   // new_rect->height = std::max(end_point.y - new_rect->top, (float)this->map_->grid()->tile_height());
   //
   // this->tile_cursor_->set_position(new_rect->left, new_rect->top);
   // this->tile_cursor_->set_size(new_rect->width, new_rect->height);
   //
   // delete new_rect;
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

void BuilderScene::set_tiles(Texture& tile_texture) {
   if (!this->tile_cursor_) {
      return;
   }

   Service::get_logger().msg(this->id_, Logger::INFO, "Adding new tiles.");

   // get bounds of tile cursor
   PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(this->tile_cursor_->get("physics"));
   if (!tc_physics) {
      Service::get_logger().msg(this->id_, Logger::ERROR, "Tile cursor does not have physics part!");
      return;
   }
   sf::FloatRect tc_bounds = tc_physics->get_bounding_box();

   for (int tile_col = tc_bounds.left; tile_col < tc_bounds.left + tc_bounds.width; tile_col += this->map_->grid()->tile_width()) {
      for (int tile_row = tc_bounds.top; tile_row < tc_bounds.top + tc_bounds.height; tile_row += this->map_->grid()->tile_height()) {
         Entity* tile = TileFactory::inst()->create_tile(
            tile_texture,
            sf::Vector2f(tile_col, tile_row),
            0,
            false
         );

         this->map_->add(tile);
      }
   }
}

void BuilderScene::remove_tiles() {
   if (!this->tile_cursor_) {
      return;
   }

   Service::get_logger().msg(this->id_, Logger::INFO, "Removing tiles.");

   // get bounds of tile cursor
   PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(this->tile_cursor_->get("physics"));
   if (!tc_physics) {
      Service::get_logger().msg(this->id_, Logger::ERROR, "Tile cursor does not have physics part!");
      return;
   }

   // get intersecting entities on map
   Map::TileList tiles = this->map_->intersects(tc_physics->get_bounding_box());
   Map::TileList::const_iterator it;
   for (it = tiles.begin(); it != tiles.end(); ++it) {
      this->map_->remove(*it);
   }
}
