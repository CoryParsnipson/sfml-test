#include "TestUIScene.h"
#include "UtilFactory.h"
#include "TextFactory.h"
#include "TextureManager.h"

#include "Game.h"
#include "MouseControlPart.h"
#include "PanelWidget.h"

TestUIScene::TestUIScene()
: Scene("TestUIScene")
{
   // create viewport layers
   this->viewport_->add("main");
   this->viewport_->add("hud");

   // load textures
   TextureManager::inst()->create_texture("ui_panel_test_tm",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(40, 0, 5, 40)
   );
   TextureManager::inst()->get_texture("ui_panel_test_tm").get_texture().setRepeated(true);

   TextureManager::inst()->create_texture("ui_panel_test_tl",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(0, 0, 40, 40)
   );

   TextureManager::inst()->create_texture("ui_panel_test_tr",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(45, 0, 40, 40)
   );

   TextureManager::inst()->create_texture("ui_panel_test_lm",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(0, 40, 40, 5)
   );
   TextureManager::inst()->get_texture("ui_panel_test_bm").get_texture().setRepeated(true);

   TextureManager::inst()->create_texture("ui_panel_test_rm",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(45, 40, 40, 5)
   );
   TextureManager::inst()->get_texture("ui_panel_test_bm").get_texture().setRepeated(true);

   TextureManager::inst()->create_texture("ui_panel_test_bm",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(40, 45, 5, 40)
   );
   TextureManager::inst()->get_texture("ui_panel_test_bm").get_texture().setRepeated(true);

   TextureManager::inst()->create_texture("ui_panel_test_br",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(45, 45, 40, 40)
   );

   TextureManager::inst()->create_texture("ui_panel_test_bl",
      "pokemon_rby_ui_panel_border.png",
      sf::IntRect(0, 45, 40, 40)
   );

   // populate entities
   this->entities_.push_back(TextFactory::inst()->create_text_entity(
      "Test UI Scene",
      "retro",
      this->viewport_->layer("hud"),
      sf::Vector2f(0, 0),
      12
   ));

   // test widget
   this->widget_ = new PanelWidget(sf::Vector2f(100, 100), sf::Vector2f(300, 200));
   this->widget_->layer(this->viewport_->layer("main"));

   // set up mouse
   this->mouse_ = UtilFactory::inst()->create_mouse(this->viewport_->layer("hud"));
   dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(dynamic_cast<PanelWidget*>(this->widget_));
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
}
