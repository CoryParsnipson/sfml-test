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

#include "TextWidget.h"
#include "PanelWidget.h"
#include "ButtonWidget.h"

#include "UtilFactory.h"
#include "TileFactory.h"
#include "TextFactory.h"
#include "Grid.h"
#include "Map.h"

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
#include "SetTilesCommand.h"
#include "RemoveTilesCommand.h"
#include "ToggleDebugInfoCommand.h"
#include "SerializeCommand.h"

#include "PlayerGamepad.h"

#include "FileChannel.h"
#include "JSONSerializer.h"
#include "OrthographicGrid.h"

BuilderScene::BuilderScene()
: Scene("BuilderScene")
, hud_camera_(new Camera("Hud Camera"))
, map_camera_(new Camera("Map Camera"))
, map_file_(new FileChannel("pkmn_map_test.txt"))
, map_(nullptr)
, center_dot_(nullptr)
, tile_cursor_(nullptr)
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
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

   // create a tile cursor
   sf::Vector2f nullvect(0, 0);
   this->tile_cursor_ = TileFactory::inst()->create_tile_cursor(nullvect, nullvect);
   this->tile_cursor_->visible(false);
   this->scene_graph_->layer(1)->add(this->tile_cursor_);

   // UI elements
   // textboxes
   this->scene_graph_->layer(2)->add(new TextWidget("Text Widget 1", "SFML_Test", sf::Vector2f(0, 0), this->fonts_.get("retro")));
   this->scene_graph_->layer(2)->add(new TextWidget("Text Widget 2", "r: reset pan position", sf::Vector2f(0, 15), this->fonts_.get("retro")));
   this->scene_graph_->layer(2)->add(new TextWidget("Text Widget 3", "g: toggle grid visibility", sf::Vector2f(0, 30), this->fonts_.get("retro")));
   this->scene_graph_->layer(2)->add(new TextWidget("Text Widget 4", "o: toggle entity hitboxes", sf::Vector2f(0, 45), this->fonts_.get("retro")));
   this->scene_graph_->layer(2)->add(new TextWidget("Text Widget 5", "del: remove tiles at selection", sf::Vector2f(0, 60), this->fonts_.get("retro")));
   this->scene_graph_->layer(2)->add(new TextWidget("Text Widget 6", "r-click: click and drag to pan", sf::Vector2f(0, 75), this->fonts_.get("retro")));

   // create fixed hud items
   Graphic* center_dot_graphic = new SpriteGraphic();
   center_dot_graphic->set_size(3, 3);
   center_dot_graphic->set_color(sf::Color(255, 104, 2));

   this->center_dot_ = UtilFactory::inst()->create_graphic(
      center_dot_graphic,
      center_dot_graphic->get_global_bounds(),
      false
   );
   this->scene_graph_->layer(2)->insert(2, this->center_dot_);

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", this->fonts_.get("retro"));
   this->scene_graph_->layer(2)->insert(2, this->fps_display_);

   // tile palette
   Widget* tile_palette = new PanelWidget("Tiles", sf::Vector2f(10, 150), sf::Vector2f(200, 830));
   this->scene_graph_->layer(2)->add(tile_palette);

   std::vector<std::string> tile_textures;
   tile_textures.push_back("tile_grass");
   tile_textures.push_back("tile_worn_grass");
   tile_textures.push_back("tile_sign");
   tile_textures.push_back("tile_dirt_ul");
   tile_textures.push_back("tile_dirt_um");
   tile_textures.push_back("tile_dirt_ur");
   tile_textures.push_back("tile_dirt_ml");
   tile_textures.push_back("tile_dirt_mm");
   tile_textures.push_back("tile_dirt_mr");
   tile_textures.push_back("tile_dirt_bl");
   tile_textures.push_back("tile_dirt_bm");
   tile_textures.push_back("tile_dirt_br");
   tile_textures.push_back("tile_water_ul");
   tile_textures.push_back("tile_water_um");
   tile_textures.push_back("tile_water_ur");
   tile_textures.push_back("tile_water_ml");
   tile_textures.push_back("tile_water_mm");
   tile_textures.push_back("tile_water_mr");
   tile_textures.push_back("tile_water_bl");
   tile_textures.push_back("tile_water_bm");
   tile_textures.push_back("tile_water_br");

   ButtonWidget* button = nullptr;
   int x_pos = 10;
   int y_pos = 10;
   
   for (std::vector<std::string>::const_iterator tile_texture_it = tile_textures.begin(); tile_texture_it != tile_textures.end(); ++tile_texture_it) {
      button = new ButtonWidget(*tile_texture_it, sf::Vector2f(x_pos, y_pos), sf::Vector2f(64, 64));
      button->set_background(new SpriteGraphic(*this->textures_.get(*tile_texture_it)));
      button->on_release(new SetTilesCommand(*this->map_, this->tile_cursor_, *this->textures_.get(*tile_texture_it)));

      tile_palette->add(button);

      if (x_pos == 84) y_pos += 74;

      if (x_pos == 10) x_pos = 84;
      else             x_pos = 10;
   }
}

BuilderScene::~BuilderScene() {
   delete this->map_file_;
}

void BuilderScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering builder state.");

   // set center dot position
   this->center_dot_->set_position(game.window().size() / 2.f);

   // set fps display position
   this->fps_display_->set_position(game.window().size().x - 60, 0);

   // change backsplash size
   this->backdrop_->set_size(game.window().size());

   // create map object (if necessary)
   if (this->map_ == nullptr) {
      JSONSerializer s;

      this->map_ = new Map();
      this->scene_graph_->layer(1)->insert(0, this->map_);

      // try to load map file
      this->map_file_->seek(0);
      std::string map_data = s.read(*this->map_file_);
      
      if (!map_data.empty()) {
         this->map_->deserialize((Serializer&)(*(&s)), game, map_data);
      } else {
         // if map file is not valid, generate empty map
         this->map_->add(new OrthographicGrid("grid"));
      };

      // TODO: make a way for map to draw grid after all it's child tiles, and remove this from here
      this->map_->grid()->visible(false);
      this->scene_graph_->layer(1)->insert(1, this->map_->grid());
   }

   // set grid size
   this->map_->grid()->size(game.window().size());

   // create player gamepad (if necessary)
   if (this->gamepad(1) == nullptr) {
      PlayerGamepad* pg = new PlayerGamepad("PlayerGamepad", this->fonts_.get("retro"));
      this->gamepad(pg);

      DefaultMouseGamepadPreset dmgp(*this->scene_graph_);
      dmgp.apply(pg);

      // create a selection rectangle entity and add it to the scene initally invisible
      Entity* selection_rect = TileFactory::inst()->create_selection_rectangle();
      selection_rect->visible(false);
      this->scene_graph_->layer(2)->insert(1, selection_rect);

      // builder scene gamepad bindings
      pg->set(new ToggleVisibleCommand(this->map_->grid()), Key::G);
      pg->set(new ToggleDebugInfoCommand(this->scene_graph_, this->fonts_.get("retro")), Key::O);
      pg->set(new ResetCameraCommand(this->map_camera_, this->map_->grid()), Key::R);
      pg->set(new SwitchSceneCommand(this, new TestUIScene()), Key::Escape);
      pg->set(new RemoveTilesCommand(*this->map_, this->tile_cursor_), Key::Delete);
      pg->set(new RemoveTilesCommand(*this->map_, this->tile_cursor_), Key::Backspace);
      pg->set(new SerializeCommand(*this->map_, std::make_shared<JSONSerializer>(3), *this->map_file_), Key::S);

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
