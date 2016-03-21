#include "TestUIScene.h"
#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"
#include "MouseControlPart.h"

TestUIScene::TestUIScene()
: Scene("TestUIScene")
{
   // create viewport layers
   this->viewport_->add("main");
   this->viewport_->add("hud");

   // populate entities
   this->entities_.push_back(TextFactory::inst()->create_text_entity(
      "Test UI Scene",
      "retro",
      this->viewport_->layer("main"),
      sf::Vector2f(0, 0),
      12
   ));

   // set up mouse
   this->mouse_ = UtilFactory::inst()->create_mouse(this->viewport_->layer("hud"));
   //dynamic_cast<MouseControlPart*>(this->mouse_->get("control"))->set_controllable(this);
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
