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

#include "PanelWidget.h"
#include "ButtonWidget.h"

#include "UtilFactory.h"
#include "TileFactory.h"
#include "TextFactory.h"
#include "TextSerializer.h"
#include "OrthographicMapBuilder.h"
#include "Grid.h"

#include "MacroCommand.h"
#include "DragCommand.h"
#include "DragTargetCommand.h"
#include "SwitchSceneCommand.h"
#include "ResetCameraCommand.h"
#include "ToggleVisibleCommand.h"
#include "ZoomCommand.h"

#include "DefaultMouseGamepadPreset.h"

#include "SetSelectionRectCommand.h"
#include "UpdateSelectionRectCommand.h"
#include "SetTileCursorCommand.h"
#include "RemoveTilesCommand.h"

#include "PlayerGamepad.h"

BuilderScene::BuilderScene()
: Scene("BuilderScene")
, map_(nullptr)
, hud_camera_(new Camera("Hud Camera"))
, map_camera_(new Camera("Map Camera"))
, map_filename_("pkmn_map_test.txt")
, center_dot_(nullptr)
, tile_cursor_(nullptr)
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
, show_debug_info_(false)
{
   // load fonts
   this->fonts_.load("retro", "retro.ttf");
   
   // load textures
   this->textures_.load("tile_grass", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 0, 64, 64));
   this->textures_.load("tile_worn_grass", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 0, 64, 64));
   this->textures_.load("tile_sign", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 0, 64, 64));
   this->textures_.load("tile_dirt_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 64, 64, 64));
   this->textures_.load("tile_dirt_um", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 64, 64, 64));
   this->textures_.load("tile_dirt_ur", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 64, 64, 64));
   this->textures_.load("tile_dirt_ml", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 128, 64, 64));
   this->textures_.load("tile_dirt_mm", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 128, 64, 64));
   this->textures_.load("tile_dirt_mr", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 128, 64, 64));
   this->textures_.load("tile_dirt_bl", "pkmn_tiles_outdoor1.png", sf::IntRect(0, 192, 64, 64));
   this->textures_.load("tile_dirt_bm", "pkmn_tiles_outdoor1.png", sf::IntRect(64, 192, 64, 64));
   this->textures_.load("tile_dirt_br", "pkmn_tiles_outdoor1.png", sf::IntRect(128, 192, 64, 64));
   this->textures_.load("tile_water_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 64, 64, 64));
   this->textures_.load("tile_water_um", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 64, 64, 64));
   this->textures_.load("tile_water_ur", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 64, 64, 64));
   this->textures_.load("tile_water_ml", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 128, 64, 64));
   this->textures_.load("tile_water_mm", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 128, 64, 64));
   this->textures_.load("tile_water_mr", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 128, 64, 64));
   this->textures_.load("tile_water_bl", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 192, 64, 64));
   this->textures_.load("tile_water_bm", "pkmn_tiles_outdoor1.png", sf::IntRect(256, 192, 64, 64));
   this->textures_.load("tile_water_br", "pkmn_tiles_outdoor1.png", sf::IntRect(320, 192, 64, 64));
   Service::get_logger().msg(this->id(), Logger::INFO, this->textures_);

   sf::VertexArray* backdrop = new sf::VertexArray(sf::TrianglesStrip, 4);
   (*backdrop)[0].position = sf::Vector2f(0, 0);
   (*backdrop)[1].position = sf::Vector2f(0, 0);
   (*backdrop)[2].position = sf::Vector2f(0, 0);
   (*backdrop)[3].position = sf::Vector2f(0, 0);

   (*backdrop)[0].color = sf::Color(50, 50, 50, 255);
   (*backdrop)[1].color = sf::Color(25, 25, 25, 255);
   (*backdrop)[2].color = sf::Color(50, 50, 50, 255);
   (*backdrop)[3].color = sf::Color(25, 25, 25, 255);

   GraphicsPart* backdrop_gp = new GraphicsPart();
   backdrop_gp->add(new VertexGraphic(backdrop));

   this->backdrop_ = new Entity("Backdrop");
   this->backdrop_->add(backdrop_gp);
   this->scene_graph_->insert(0, this->backdrop_); 

   // create "map layers" using a new camera
   this->scene_graph_->insert(1, this->map_camera_); // layer for map
   this->scene_graph_->insert(2, this->hud_camera_); // layer for hud 

   // build the map
   this->serializer_ = new TextSerializer();
   this->serializer_->open_infile(this->map_filename_);

   MapBuilder* map_builder = new OrthographicMapBuilder();
   map_builder->set_serializer(this->serializer_);
   map_builder->grid_font(this->fonts_.get("retro"));
   map_builder->textures(&this->textures_);
   map_builder->build();

   this->map_ = map_builder->get_map();
   this->scene_graph_->layer(1)->add(this->map_);

   this->map_->grid()->visible(false);
   this->scene_graph_->layer(1)->add(this->map_->grid());

   delete map_builder;

   // create a selection rectangle entity and add it to the scene initally invisible
   Entity* selection_rect = TileFactory::inst()->create_selection_rectangle();
   selection_rect->visible(false);
   this->scene_graph_->layer(2)->insert(1, selection_rect);

   // create a tile cursor
   sf::Vector2f nullvect(0, 0);
   this->tile_cursor_ = TileFactory::inst()->create_tile_cursor(nullvect, nullvect);
   this->tile_cursor_->visible(false);
   this->scene_graph_->layer(1)->add(this->tile_cursor_);

   // create fixed hud items
   Entity* t = TextFactory::inst()->create_text_entity("SFML_Test", this->fonts_.get("retro"));
   this->scene_graph_->layer(2)->insert(2, t);

   t = TextFactory::inst()->create_text_entity("r: reset pan position", this->fonts_.get("retro"), sf::Vector2f(0, 15));
   this->scene_graph_->layer(2)->insert(2, t);

   t = TextFactory::inst()->create_text_entity("g: toggle grid visibility", this->fonts_.get("retro"), sf::Vector2f(0, 30));
   this->scene_graph_->layer(2)->insert(2, t);

   t = TextFactory::inst()->create_text_entity("o: toggle entity hitboxes", this->fonts_.get("retro"), sf::Vector2f(0, 45));
   this->scene_graph_->layer(2)->insert(2, t);

   t = TextFactory::inst()->create_text_entity("1: add green tiles at selection", this->fonts_.get("retro"), sf::Vector2f(0, 60));
   this->scene_graph_->layer(2)->insert(2, t);

   t = TextFactory::inst()->create_text_entity("2: add blue tiles at selection", this->fonts_.get("retro"), sf::Vector2f(0, 75));
   this->scene_graph_->layer(2)->insert(2, t);

   t = TextFactory::inst()->create_text_entity("del: remove tiles at selection", this->fonts_.get("retro"), sf::Vector2f(0, 90));
   this->scene_graph_->layer(2)->insert(2, t);

   t = TextFactory::inst()->create_text_entity("right click: click and drag to pan", this->fonts_.get("retro"), sf::Vector2f(0, 105));
   this->scene_graph_->layer(2)->insert(2, t);

   Graphic* center_dot_graphic = new SpriteGraphic();
   center_dot_graphic->set_size(3, 3);
   center_dot_graphic->set_color(sf::Color(255, 104, 2));

   this->center_dot_ = UtilFactory::inst()->create_graphic(
      center_dot_graphic,
      center_dot_graphic->get_global_bounds(),
      this->show_debug_info_
   );
   this->scene_graph_->layer(2)->insert(2, this->center_dot_);

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", this->fonts_.get("retro"));
   this->scene_graph_->layer(2)->insert(2, this->fps_display_);

   // create UI elements
   Widget* tile_palette = new PanelWidget("Tiles", sf::Vector2f(10, 150), sf::Vector2f(200, 400));
   this->scene_graph_->layer(2)->add(tile_palette);

   ButtonWidget* button = nullptr;

   button = new ButtonWidget(this->textures_.get("tile_grass")->id(), sf::Vector2f(10, 10), sf::Vector2f(64, 64));
   button->set_background(new SpriteGraphic(*this->textures_.get("tile_grass")));
   tile_palette->add(button);

   // create player gamepad
   PlayerGamepad* pg = new PlayerGamepad("PlayerGamepad", this->fonts_.get("retro"));
   this->gamepad(pg);

   DefaultMouseGamepadPreset dmgp(*this->scene_graph_);
   dmgp.apply(pg);

   // builder scene gamepad bindings
   pg->set(new ToggleVisibleCommand(this->map_->grid()), Key::G);
   pg->set(new ResetCameraCommand(this->map_camera_, this->map_->grid()), Key::R);
   pg->set(new SwitchSceneCommand(this, new TestUIScene()), Key::Escape);
   pg->set(new RemoveTilesCommand(*this->map_, this->tile_cursor_), Key::Delete);
   pg->set(new RemoveTilesCommand(*this->map_, this->tile_cursor_), Key::Backspace);

   std::shared_ptr<UpdateSelectionRectCommand> usr = std::make_shared<UpdateSelectionRectCommand>(selection_rect);

   DragCommand* drag_map_camera = new DragCommand(pg);
   DragCommand* drag_grid = new DragCommand(pg);

   MacroCommand* drag_command = new MacroCommand("DragMacroCommand");
   drag_command->add(drag_map_camera);
   drag_command->add(drag_grid);
   drag_command->add(new SetSelectionRectCommand(usr, pg, selection_rect, false));
   
   MacroCommand* on_left_mouse_release = new MacroCommand("LeftMouseReleaseCommand");
   on_left_mouse_release->add(new SetSelectionRectCommand(usr, pg, selection_rect, false, false, true));
   on_left_mouse_release->add(new SetTileCursorCommand(*this->map_->grid(), usr, this->tile_cursor_));

   MacroCommand* on_right_mouse_click = new MacroCommand("SetDragTargetsMacroCommand");
   on_right_mouse_click->add(new DragTargetCommand(drag_map_camera, this->map_camera_));
   on_right_mouse_click->add(new DragTargetCommand(drag_grid, this->map_->grid()));

   MacroCommand* on_right_mouse_release = new MacroCommand("RemoveDragTargetsMacroCommand");
   on_right_mouse_release->add(new DragTargetCommand(drag_map_camera, nullptr));
   on_right_mouse_release->add(new DragTargetCommand(drag_grid, nullptr));

   this->map_camera_->clickable(true); // make map camera clickable for tile selection and panning and stuff
   this->map_camera_->on_left_click(new SetSelectionRectCommand(usr, pg, selection_rect, true, true, false));
   this->map_camera_->on_left_release(on_left_mouse_release);

   this->map_camera_->on_right_click(on_right_mouse_click);
   this->map_camera_->on_right_release(on_right_mouse_release);

   this->map_camera_->on_mouse_move(drag_command);
   this->map_camera_->on_mouse_wheel(new ZoomCommand(this->map_camera_, this->map_->grid(), pg));
}

BuilderScene::~BuilderScene() {
   delete this->serializer_;
}

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering builder state.");

   // set center dot position
   this->center_dot_->set_position(game.window().size() / 2.f);

   // set fps display position
   this->fps_display_->set_position(game.window().size().x - 60, 0);

   // change backsplash size
   this->backdrop_->set_size(game.window().size());

   // set grid size
   this->map_->grid()->size(game.window().size());
}

void BuilderScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting builder state.");
}

void BuilderScene::update(Game& game, Scene* scene) {
   Scene::update(game, scene);

   // calculate and show FPS
   if (!this->frame_count) {
      this->update_fps(game.settings);
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

   // change grid size
   this->map_->grid()->set_position(-1.f * this->map_camera_->get_pan_delta());

   // reposition fps display
   this->fps_display_->set_position(game.window().size().x - 60, 0);

   // change backsplash size
   this->backdrop_->set_size(game.window().size());

   // set grid size
   this->map_->grid()->size(game.window().size());
}

void BuilderScene::process(Game& game, KeyPressInputEvent& e) {
//   switch (e.key) {
//   case Key::Num1:
//   case Key::Numpad1:
//      this->set_tiles(this->textures_.get("tile_grass"));
//   break;
//   case Key::Num2:
//   case Key::Numpad2:
//      this->set_tiles(this->textures_.get("tile_worn_grass"));
//   break;
//   case Key::Num3:
//   case Key::Numpad3:
//      this->set_tiles(this->textures_.get("tile_dirt_ul"));
//   break;
//   case Key::Num4:
//   case Key::Numpad4:
//      this->set_tiles(this->textures_.get("tile_dirt_um"));
//   break;
//   case Key::Num5:
//   case Key::Numpad5:
//      this->set_tiles(this->textures_.get("tile_dirt_ur"));
//   break;
//   case Key::Num6:
//   case Key::Numpad6:
//      this->set_tiles(this->textures_.get("tile_dirt_ml"));
//   break;
//   case Key::Num7:
//   case Key::Numpad7:
//      this->set_tiles(this->textures_.get("tile_dirt_mm"));
//   break;
//   case Key::Num8:
//   case Key::Numpad8:
//      this->set_tiles(this->textures_.get("tile_dirt_mr"));
//   break;
//   case Key::Num9:
//   case Key::Numpad9:
//      this->set_tiles(this->textures_.get("tile_dirt_bl"));
//   break;
//   case Key::Slash:
//      this->set_tiles(this->textures_.get("tile_dirt_bm"));
//   break;
//   case Key::Multiply:
//      this->set_tiles(this->textures_.get("tile_dirt_br"));
//   break;
//   case Key::O:
//      this->toggle_debug_info();
//   break;
//   case Key::S:
//      Service::get_logger().msg(this->id_, Logger::INFO, "Writing map to file '" + this->map_filename_ + "'");
//      this->map_->serialize(*this->serializer_);
//   break;
//   }
}

void BuilderScene::update_fps(Settings& settings) {
   this->last_frame_time = (((float)this->frame_measurement_interval / this->clock.getElapsedTime().asSeconds()) * settings.framerate_smoothing())
                           + (this->last_frame_time * (1.0 - settings.framerate_smoothing()));
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

   // TODO: can replace this with visitor
   SceneObject::prefix_iterator it;
   for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
      auto e = dynamic_cast<Entity*>(*it);
      if (!e) {
         continue;
      }

      if (this->show_debug_info_) {
         e->add(new DebugPart("debug", this->fonts_.get("retro")));
      } else {
         e->remove("debug");
      }
   }
}

void BuilderScene::set_tiles(Texture& tile_texture) {
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
            false
         );

         this->map_->add(tile);
      }
   }
}
