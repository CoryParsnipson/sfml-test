#include "TestUIScene.h"
#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"
#include "GraphicsPart.h"

#include "PanelWidget.h"
#include "TextWidget.h"
#include "ButtonWidget.h"

#include "WidgetEventCommand.h"
#include "MoveCommand.h"

#include "PlayerGamepad.h"

TestUIScene::TestUIScene()
: Scene("TestUIScene")
, ui_camera_(new Camera("UI Camera"))
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
{
   // load fonts
   this->fonts_.load("retro", "retro.ttf");
   
   // load textures
   this->textures_.load("ui_resize_handle", "ui_panel_test.png", sf::IntRect(30, 0, 10, 10));
   this->textures_.load("tile_water_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 64, 64, 64));
   Service::get_logger().msg(this->id(), Logger::INFO, this->textures_);

   // create camera just for UI
   this->scene_graph_->add(this->ui_camera_);

   // populate entities
   this->scene_graph_->add(
      TextFactory::inst()->create_text_entity(
         "Test UI Scene",
         this->fonts_.get("retro"),
         sf::Vector2f(0, 0),
         12
   ));

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", this->fonts_.get("retro"));
   this->scene_graph_->add(this->fps_display_);

   // test widget
   this->widget_ = new PanelWidget("Test Panel 1", sf::Vector2f(100, 100), sf::Vector2f(300, 200));
   this->scene_graph_->layer(0)->add(this->widget_);

   Widget* tw = new TextWidget("Text Widget 1", "PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS", this->fonts_.get("retro"));
   tw->set_size(sf::Vector2f(300, 200));
   this->widget_->add(tw);
   
   ButtonWidget* bw = new ButtonWidget("Button Widget 1", sf::Vector2f(10, 30), sf::Vector2f(30, 15));
   bw->set_background(new SpriteGraphic(*this->textures_.get("tile_water_ul")));
   this->widget_->add(bw);

   bw->action(new MoveCommand(this->ui_camera_, sf::Vector2f(10, 50)));

   // create player controls
   PlayerGamepad* pg = new PlayerGamepad("PlayerGamepad", this->fonts_.get("retro"));
   this->gamepad(pg);
   
   pg->set(new WidgetEventCommand(WidgetOp::MouseClick, this->scene_graph_, pg), MouseButton::Left);
   pg->set(new WidgetEventCommand(WidgetOp::MouseMove, this->scene_graph_, pg), MouseAction::Move);
}

TestUIScene::~TestUIScene() {
}

void TestUIScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering test UI menu state.");

   // reposition fps display
   this->fps_display_->set_position(game.window().size().x - 60, 0);
}

void TestUIScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting test UI menu state.");
}

void TestUIScene::update(Game& game, Scene* scene) {
   Scene::update(game, scene);

   // calculate and show FPS
   if (!this->frame_count) {
      this->update_fps();
   }
   this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
}

void TestUIScene::update_fps() {
   this->last_frame_time = (((float)this->frame_measurement_interval / this->clock.getElapsedTime().asSeconds()) * Settings::Instance()->FRAMERATE_SMOOTHING)
                           + (this->last_frame_time * (1.0 - Settings::Instance()->FRAMERATE_SMOOTHING));
   this->clock.restart();

   // update fps entity
   GraphicsPart* fps_graphic = dynamic_cast<GraphicsPart*>(fps_display_->get("graphics"));
   if (fps_graphic) {
      fps_graphic->get(0)->set_string("FPS: " + std::to_string(this->last_frame_time));
   }
}

void TestUIScene::process(Game& game, ResizeInputEvent& e) {
   Scene::process(game, e);

   // reposition fps display
   this->fps_display_->set_position(game.window().size().x - 60, 0);
}

void TestUIScene::process(Game& game, KeyPressInputEvent& e) {
   if (e.key == Key::Space) {
      game.unload_scene();
   }
}

void TestUIScene::process(Game& game, MouseButtonInputEvent& e) {}
