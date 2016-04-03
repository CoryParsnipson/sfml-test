#include "TestUIScene.h"
#include "UtilFactory.h"
#include "TextFactory.h"
#include "TextureManager.h"

#include "Game.h"
#include "MouseControlPart.h"
#include "GraphicsPart.h"

#include "PanelWidget.h"
#include "TextWidget.h"

TestUIScene::TestUIScene()
: Scene("TestUIScene")
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
{
   // create viewport layers
   this->viewport_->add("main");
   this->viewport_->add("hud");

   // load textures
   TextureManager::inst()->create_texture("ui_resize_handle", "ui_panel_test.png", sf::IntRect(30, 0, 10, 10));
   TextureManager::inst()->print();

   // populate entities
   this->entities_.push_back(TextFactory::inst()->create_text_entity(
      "Test UI Scene",
      "retro",
      this->viewport_->layer("hud"),
      sf::Vector2f(0, 0),
      12
   ));

   // test widget
   this->widget_ = new PanelWidget(sf::Vector2f(100, 100), sf::Vector2f(300, 200), nullptr, true, true);
   this->widget_->layer(this->viewport_->layer("main"));

   Widget* tw = new TextWidget("PENIS");
   tw->layer(this->viewport_->layer("main"));
   this->widget_->add(tw);

   // set up mouse
   this->mouse_ = UtilFactory::inst()->create_mouse(this->viewport_->layer("hud"));
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(dynamic_cast<PanelWidget*>(this->widget_));

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", "retro", this->viewport_->layer("hud"));
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);
   this->entities_.push_back(this->fps_display_);
}

TestUIScene::~TestUIScene() {
}

void TestUIScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering test UI menu state.");
   Service::get_input().registerInputListener(dynamic_cast<InputListener*>(this->mouse_->get("control")));
}

void TestUIScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting test UI menu state.");
   Service::get_input().unregisterInputListener(dynamic_cast<InputListener*>(this->mouse_->get("control")));
}

void TestUIScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);

   this->mouse_->update(game, this);

   // calculate and show FPS
   if (!this->frame_count) {
      this->update_fps();
   }
   this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
}

void TestUIScene::process(Game& game, WindowResizeCommand& c) {
   Scene::process(game, c);

   sf::Vector2f new_size(c.width, c.height);
   sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

   // reposition fps display
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);
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
