#include "BuilderScene.h"
#include "TestUIScene.h"

#include "Game.h"

#include "Draw.h"
#include "RenderSurface.h"
#include "Texture.h"

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

#include "MoveCameraCommand.h"

BuilderScene::BuilderScene()
: Scene("BuilderScene")
, map_(nullptr)
, map_camera_(new Camera("Map Camera", sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height())))
, map_filename_("pkmn_map_test.txt")
, mouse_(nullptr)
, center_dot_(nullptr)
, selection_rectangle_(nullptr)
, tile_cursor_(nullptr)
, map_grid_(nullptr)
, move_camera_left(new MoveCameraCommand(*this->map_camera_, sf::Vector2f(-10, 0)))
, move_camera_right(new MoveCameraCommand(*this->map_camera_, sf::Vector2f(10, 0)))
, move_camera_up(new MoveCameraCommand(*this->map_camera_, sf::Vector2f(0, -10)))
, move_camera_down(new MoveCameraCommand(*this->map_camera_, sf::Vector2f(0, 10)))
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
, show_debug_info_(false)
{
   // load textures
   TextureManager::inst()->create_texture("tile_grass", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 0, 64, 64));
   TextureManager::inst()->create_texture("tile_worn_grass", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 0, 64, 64));
   TextureManager::inst()->create_texture("tile_sign", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 0, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 64, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_um", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 64, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_ur", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 64, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_ml", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 128, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_mm", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 128, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_mr", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 128, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_bl", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 192, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_bm", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 192, 64, 64));
   TextureManager::inst()->create_texture("tile_dirt_br", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 192, 64, 64));
   TextureManager::inst()->create_texture("tile_water_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 64, 64, 64));
   TextureManager::inst()->create_texture("tile_water_um", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 64, 64, 64));
   TextureManager::inst()->create_texture("tile_water_ur", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 64, 64, 64));
   TextureManager::inst()->create_texture("tile_water_ml", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 128, 64, 64));
   TextureManager::inst()->create_texture("tile_water_mm", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 128, 64, 64));
   TextureManager::inst()->create_texture("tile_water_mr", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 128, 64, 64));
   TextureManager::inst()->create_texture("tile_water_bl", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 192, 64, 64));
   TextureManager::inst()->create_texture("tile_water_bm", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 192, 64, 64));
   TextureManager::inst()->create_texture("tile_water_br", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 192, 64, 64));
   TextureManager::inst()->print();

   // create "map layers" using a new camera
   this->scene_graph_[1] = new CameraSceneGraphNode(*this->map_camera_); // layer for map
   this->scene_graph_[2] = new CameraSceneGraphNode(*this->map_camera_); // layer for tile cursor

   this->scene_graph_[3] = new CameraSceneGraphNode(*this->camera_); // layer for hud
   this->scene_graph_[4] = new CameraSceneGraphNode(*this->camera_); // layer for mouse

   // build the map
   this->serializer_ = new TextSerializer();
   this->serializer_->open_infile(this->map_filename_);

   MapBuilder* map_builder = new FlatMapBuilder();
   map_builder->set_serializer(this->serializer_);
   map_builder->build();

   this->map_ = map_builder->get_map();
   this->scene_graph_[1]->add(new DrawableSceneGraphNode(*this->map_));

   this->map_grid_ = new DrawableSceneGraphNode(*this->map_->grid());
   this->map_grid_->visible(false);
   this->scene_graph_[2]->add(this->map_grid_);

   sf::VertexArray* backdrop = new sf::VertexArray(sf::TrianglesStrip, 4);
   (*backdrop)[0].position = sf::Vector2f(0, 0);
   (*backdrop)[1].position = sf::Vector2f(0, Settings::Instance()->cur_height());
   (*backdrop)[2].position = sf::Vector2f(Settings::Instance()->cur_width(), 0);
   (*backdrop)[3].position = sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height());

   (*backdrop)[0].color = sf::Color(50, 50, 50, 255);
   (*backdrop)[1].color = sf::Color(25, 25, 25, 255);
   (*backdrop)[2].color = sf::Color(50, 50, 50, 255);
   (*backdrop)[3].color = sf::Color(25, 25, 25, 255);
   this->backdrop_ = new VertexGraphic(backdrop);
   this->scene_graph_[0]->add(new DrawableSceneGraphNode(*this->backdrop_));

   delete map_builder;

   // initialize entities
   this->mouse_ = UtilFactory::inst()->create_mouse();
   this->scene_graph_[4]->add(new EntitySceneGraphNode(*this->mouse_));

   // let our mouse controller manipulate this scene
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(this);

   // create a selection rectangle entity and add it to the scene initally invisible
   this->selection_rectangle_ = new EntitySceneGraphNode(
      *TileFactory::inst()->create_selection_rectangle(),
      sf::RenderStates::Default,
      false
   );
   this->scene_graph_[3]->add(this->selection_rectangle_);

   // create a tile cursor
   sf::Vector2f nullvect(0, 0);
   this->tile_cursor_ = new EntitySceneGraphNode(
      *TileFactory::inst()->create_tile_cursor(
         nullvect,
         nullvect
      ),
      sf::RenderStates::Default,
      false
   );
   this->scene_graph_[2]->add(this->tile_cursor_);

   // create fixed hud items
   Entity* t = TextFactory::inst()->create_text_entity("SFML_Test", "retro");
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   t = TextFactory::inst()->create_text_entity("r: reset pan position", "retro", sf::Vector2f(0, 15));
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   t = TextFactory::inst()->create_text_entity("g: toggle grid visibility", "retro", sf::Vector2f(0, 30));
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   t = TextFactory::inst()->create_text_entity("o: toggle entity hitboxes", "retro", sf::Vector2f(0, 45));
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   t = TextFactory::inst()->create_text_entity("1: add green tiles at selection", "retro", sf::Vector2f(0, 60));
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   t = TextFactory::inst()->create_text_entity("2: add blue tiles at selection", "retro", sf::Vector2f(0, 75));
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   t = TextFactory::inst()->create_text_entity("del: remove tiles at selection", "retro", sf::Vector2f(0, 90));
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   t = TextFactory::inst()->create_text_entity("right click: click and drag to pan", "retro", sf::Vector2f(0, 105));
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*t));

   Graphic* center_dot_graphic = new SpriteGraphic();
   center_dot_graphic->set_size(3, 3);
   center_dot_graphic->set_color(sf::Color(255, 104, 2));
   center_dot_graphic->set_position(Settings::Instance()->cur_width() / 2, Settings::Instance()->cur_height() / 2);

   this->center_dot_ = UtilFactory::inst()->create_graphic(
      center_dot_graphic,
      center_dot_graphic->get_global_bounds(),
      this->show_debug_info_
   );
   this->scene_graph_[3]->add(new DrawableSceneGraphNode(*this->center_dot_));

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", "retro");
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);
   this->scene_graph_[3]->add(new EntitySceneGraphNode(*this->fps_display_));
}

BuilderScene::~BuilderScene() {
   delete this->map_;
   delete this->serializer_;
   delete this->map_camera_;
   delete this->selection_rectangle_->get_entity();
   delete this->tile_cursor_->get_entity();
}

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering builder start menu state.");
   Service::get_input().attach(*dynamic_cast<InputListener*>(this->mouse_->get("control")));
}

void BuilderScene::exit(Game& game) {
   Service::get_input().detach(*dynamic_cast<InputListener*>(this->mouse_->get("control")));
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

void BuilderScene::process(Game& game, CloseInputEvent& e) {
   game.unload_scene();
}

void BuilderScene::process(Game& game, ResizeInputEvent& e) {
   Scene::process(game, e);

   sf::Vector2f new_size(e.width, e.height);
   sf::Vector2f new_center(e.width / 2.f, e.height / 2.f);

   // reposition center dot
   this->center_dot_->set_position(new_center);

   // update grid
   this->map_->grid()->set_position(-1.f * this->map_camera_->get_pan_delta());

   // reposition fps display
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);

   // update backsplash
   this->backdrop_->set_size(Settings::Instance()->cur_width(), Settings::Instance()->cur_height());

   // update map camera
   this->map_camera_->set_size(sf::Vector2f(e.width, e.height));
   this->map_camera_->set_center(sf::Vector2f(e.width / 2.f, e.height / 2.f));
}

void BuilderScene::process(Game& game, KeyPressInputEvent& e) {
   switch (e.key) {
   case Key::R:
      // reset grid too (encapsulate this in grid class?)
      this->map_->grid()->set_scale(1.f);
      this->map_->grid()->move(this->map_camera_->get_pan_delta()); // move the grid back too

      this->map_camera_->reset_pan();
      this->map_camera_->reset_zoom();
   break;
   case Key::Num1:
   case Key::Numpad1:
      this->set_tiles(TextureManager::inst()->get_texture("tile_grass"));
   break;
   case Key::Num2:
   case Key::Numpad2:
      this->set_tiles(TextureManager::inst()->get_texture("tile_worn_grass"));
   break;
   case Key::Num3:
   case Key::Numpad3:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_ul"));
   break;
   case Key::Num4:
   case Key::Numpad4:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_um"));
   break;
   case Key::Num5:
   case Key::Numpad5:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_ur"));
   break;
   case Key::Num6:
   case Key::Numpad6:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_ml"));
   break;
   case Key::Num7:
   case Key::Numpad7:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_mm"));
   break;
   case Key::Num8:
   case Key::Numpad8:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_mr"));
   break;
   case Key::Num9:
   case Key::Numpad9:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_bl"));
   break;
   case Key::Slash:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_bm"));
   break;
   case Key::Multiply:
      this->set_tiles(TextureManager::inst()->get_texture("tile_dirt_br"));
   break;
   case Key::O:
      this->toggle_debug_info();
   break;
   case Key::G:
      // toggle map grid visibility
      this->map_grid_->visible(!this->map_grid_->visible());
   break;
   case Key::Delete:
   case Key::Backspace:
      this->remove_tiles();
   break;
   case Key::S:
      Service::get_logger().msg(this->id_, Logger::INFO, "Writing map to file '" + this->map_filename_ + "'");
      this->map_->serialize(*this->serializer_);
   break;
   case Key::Escape:
      // load super secret test ui scene
      game.switch_scene(new TestUIScene());
   break;
   case Key::Left:
      this->move_camera_left->execute();
      this->map_->grid()->move(-this->move_camera_left->get_delta()); // reverse pan the grid so it stays in place
   break;
   case Key::Right:
      this->move_camera_right->execute();
      this->map_->grid()->move(-this->move_camera_right->get_delta()); // reverse pan the grid so it stays in place
   break;
   case Key::Up:
      this->move_camera_up->execute();
      this->map_->grid()->move(-this->move_camera_up->get_delta()); // reverse pan the grid so it stays in place
   break;
   case Key::Down:
      this->move_camera_down->execute();
      this->map_->grid()->move(-this->move_camera_down->get_delta()); // reverse pan the grid so it stays in place
   break;
   default:
      // do nothing
   break;
   }
}

void BuilderScene::process(Game& game, MouseMoveInputEvent& c) {}
void BuilderScene::process(Game& game, MouseWheelInputEvent& c) {}
void BuilderScene::process(Game& game, MouseButtonInputEvent& c) {}

void BuilderScene::drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta) {
   if (button == MouseButton::Left) {
      this->update_selection_rect(this->click_press_pos_, pos);
   } else if (button == MouseButton::Right) {
      this->map_camera_->drag(button, pos, delta); // pan only the map layers
      this->map_->grid()->move(-delta); // reverse pan the grid so it stays in place

      // if the selection rectangle is visible, update it
      if (this->selection_rectangle_->visible()) {
         this->update_selection_rect(this->click_press_pos_, pos);
      }
   }
}

float BuilderScene::get_scale() {
   return this->map_camera_->get_scale();
}

void BuilderScene::set_scale(float factor) {
   this->map_camera_->set_scale(factor);

   factor = std::max(factor, Camera::ZOOM_FACTOR_MIN);
   factor = std::min(factor, Camera::ZOOM_FACTOR_MAX);
   this->map_->grid()->set_scale(factor);
}

void BuilderScene::click(MouseButton button, MouseButtonState state, sf::Vector2f pos) {
   if (button == MouseButton::Left) {
      if (state == MouseButtonState::Pressed) {
         this->click_press_pos_ = pos;

         this->selection_rectangle_->visible(true);
         this->update_selection_rect(this->click_press_pos_, this->click_press_pos_);
      } else if (state == MouseButtonState::Released) {
         this->selection_rectangle_->visible(false);
         this->update_tile_cursor(this->click_press_pos_, pos);
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
   this->show_debug_info_ = !this->show_debug_info_;
   Service::get_logger().msg(this->id(), Logger::INFO, "Debug info: " + std::string(this->show_debug_info_ ? "SHOW" : "HIDE"));

   SceneGraph::iterator it;
   for (it = this->scene_graph_.begin(); it != this->scene_graph_.end(); ++it) {
      SceneGraphNode::prefix_iterator node_it;
      for (node_it = it->second->begin(); node_it != it->second->end(); ++node_it) {
         auto e = dynamic_cast<EntitySceneGraphNode*>(*node_it);
         if (!e) {
            continue;
         }

         if (this->show_debug_info_) {
            e->get_entity()->add(new DebugPart());
         } else {
            e->get_entity()->remove("debug");
         }
      }
   }
}

void BuilderScene::update_selection_rect(sf::Vector2f& origin_click, sf::Vector2f& mouse_pos) {
   sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(origin_click, mouse_pos);

   this->selection_rectangle_->get_entity()->set_position(new_rect->left, new_rect->top);
   this->selection_rectangle_->get_entity()->set_size(new_rect->width, new_rect->height);

   delete new_rect;
}

void BuilderScene::update_tile_cursor(sf::Vector2f& one, sf::Vector2f& two) {
   // compensate for main viewport layer zoom
   sf::Vector2f offset_one = this->map_camera_->get_scale() * (one - this->camera_->get_center()) + this->camera_->get_center();
   sf::Vector2f offset_two = this->map_camera_->get_scale() * (two - this->camera_->get_center()) + this->camera_->get_center();

   // compensate for the panning of main viewport layer
   sf::Vector2f pan_delta = this->map_camera_->get_pan_delta();
   offset_one -= pan_delta;
   offset_two -= pan_delta;

   sf::FloatRect* new_rect = UtilFactory::inst()->create_float_rect(offset_one, offset_two);

   bool is_drag_gesture = (new_rect->width >= this->map_->grid()->tile_width() / 3.f ||
                           new_rect->height >= this->map_->grid()->tile_height() / 3.f);

   if (!is_drag_gesture && this->tile_cursor_->visible() && this->tile_cursor_->get_entity()->intersects(offset_one)) {
      this->tile_cursor_->visible(false);
      delete new_rect;
      return;
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

   this->tile_cursor_->get_entity()->set_position(new_rect->left, new_rect->top);
   this->tile_cursor_->get_entity()->set_size(new_rect->width, new_rect->height);
   this->tile_cursor_->visible(true);

   delete new_rect;
}

void BuilderScene::set_tiles(Texture& tile_texture) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Adding new tiles.");

   // get bounds of tile cursor
   Entity* tc_entity = this->tile_cursor_->get_entity();
   PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(tc_entity->get("physics"));
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
            false
         );

         this->map_->add(tile);
      }
   }
}

void BuilderScene::remove_tiles() {
   Service::get_logger().msg(this->id_, Logger::INFO, "Removing tiles.");

   // get bounds of tile cursor
   Entity* tc_entity = this->tile_cursor_->get_entity();
   PhysicsPart* tc_physics = dynamic_cast<PhysicsPart*>(tc_entity->get("physics"));
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
