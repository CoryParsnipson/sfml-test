#include "TestUIScene.h"
#include "UtilFactory.h"
#include "TextFactory.h"
#include "TextureManager.h"

#include "Game.h"
#include "MouseControlPart.h"
#include "GraphicsPart.h"

#include "PanelWidget.h"
#include "TextWidget.h"
#include "ButtonWidget.h"

#include "CameraSceneGraphNode.h"
#include "DrawableSceneGraphNode.h"
#include "EntitySceneGraphNode.h"

TestUIScene::TestUIScene()
: Scene("TestUIScene")
, last_frame_time(0)
, frame_measurement_interval(6)
, frame_count(0)
{
   // load textures
   TextureManager::inst()->create_texture("ui_resize_handle", "ui_panel_test.png", sf::IntRect(30, 0, 10, 10));
   TextureManager::inst()->create_texture("tile_water_ul", "pkmn_tiles_outdoor1.png", sf::IntRect(192, 64, 64, 64));
   TextureManager::inst()->print();

   this->hud_camera_ = new Camera("Hud Camera", sf::Vector2f(Settings::Instance()->cur_width(), Settings::Instance()->cur_height()));

   // create layers
   this->scene_graph_[1] = new CameraSceneGraphNode(*this->hud_camera_); // hud layer
   this->scene_graph_[2] = new CameraSceneGraphNode(*this->hud_camera_); // mouse layer

   // populate entities
   this->scene_graph_[1]->add(new EntitySceneGraphNode(
      *TextFactory::inst()->create_text_entity(
         "Test UI Scene",
         "retro",
         sf::Vector2f(0, 0),
         12
   )));

   // test widget
   this->widget_ = new PanelWidget(sf::Vector2f(100, 100), sf::Vector2f(300, 200), true, true);

   Widget* tw = new TextWidget("PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS PENIS");
   tw->set_size(sf::Vector2f(300, 200));
   this->widget_->add(tw);
   this->scene_graph_[0]->add(new DrawableSceneGraphNode(*this->widget_));
   
   ButtonWidget* bw = new ButtonWidget(sf::Vector2f(10, 30), sf::Vector2f(30, 15));
   bw->set_background(new SpriteGraphic(TextureManager::inst()->get_texture("tile_water_ul")));
   this->widget_->add(bw);

   // set up mouse
   this->mouse_ = UtilFactory::inst()->create_mouse();
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(dynamic_cast<PanelWidget*>(this->widget_));
   this->scene_graph_[2]->add(new EntitySceneGraphNode(*this->mouse_));

   this->fps_display_ = TextFactory::inst()->create_text_entity("FPS: ", "retro");
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);
   this->scene_graph_[1]->add(new EntitySceneGraphNode(*this->fps_display_));
}

TestUIScene::~TestUIScene() {
   delete this->widget_;
}

void TestUIScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering test UI menu state.");
   Service::get_input().attach(*dynamic_cast<InputListener*>(this->mouse_->get("control")));
}

void TestUIScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting test UI menu state.");
   Service::get_input().detach(*dynamic_cast<InputListener*>(this->mouse_->get("control")));
}

void TestUIScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);

   // calculate and show FPS
   if (!this->frame_count) {
      this->update_fps();
   }
   this->frame_count = (this->frame_count + 1) % this->frame_measurement_interval;
}

void TestUIScene::process(Game& game, ResizeInputEvent& e) {
   Scene::process(game, e);

   sf::Vector2f new_size(e.width, e.height);
   sf::Vector2f new_center(e.width / 2.f, e.height / 2.f);

   // reposition fps display
   this->fps_display_->set_position(Settings::Instance()->cur_width() - 60, 0);

   // adjust hud camera
   this->hud_camera_->set_size(sf::Vector2f(e.width, e.height));
   this->hud_camera_->set_center(sf::Vector2f(e.width / 2.f, e.height / 2.f));
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
