#include "StartMenuScene.h"
#include "BuilderScene.h"
#include "TestUIScene.h"

#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"

#include "Draw.h"
#include "GraphicsPart.h"
#include "DebugPart.h"
#include "SceneObject.h"

#include "PlayerGamepad.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
, show_debug_info_(false)
{
   // load fonts
   this->fonts_.load("retro", "retro.ttf");
   
   // populate entities
   SceneObject* node = TextFactory::inst()->create_text_entity(
      "SFML TEST",
      this->fonts_.get("retro"),
      sf::Vector2f(0, 0),
      36,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "main menu",
      this->fonts_.get("retro"),
      sf::Vector2f(0, 45),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "(Press SPACE or ENTER)",
      this->fonts_.get("retro"),
      sf::Vector2f(0, 60),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   );
   this->scene_graph_->add(node);
   this->gamepad(new PlayerGamepad("PlayerGamepad", this->fonts_.get("retro")));
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering game start menu state.");
   this->camera_->reset_pan();
   this->camera_->move(this->camera_->get_center() - game.window().size());
}

void StartMenuScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::process(Game& game, ResizeInputEvent& e) {
   Scene::process(game, e);

   // re-adjust centering
   this->camera_->reset_pan();
   this->camera_->move(this->camera_->get_center() - game.window().size());
}

void StartMenuScene::process(Game& game, KeyPressInputEvent& e) {
   if (e.key == Key::Space || e.key == Key::Return) {
      game.switch_scene(new BuilderScene());
   } else if (e.key == Key::Escape) {
      // go to super special awesome test scene
      game.switch_scene(new TestUIScene());
   } else if (e.key == Key::O) {
      this->toggle_debug_info();
   }
}

void StartMenuScene::toggle_debug_info() {
   this->show_debug_info_ = !this->show_debug_info_;
   Service::get_logger().msg(this->id(), Logger::INFO, "Debug info: " + std::string(this->show_debug_info_ ? "SHOW" : "HIDE"));

   // TODO: this could be replaced with a visitor
   SceneObject::prefix_iterator it;
   for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
      auto e = dynamic_cast<Entity*>(*it);
      if (!e) {
         continue;
      }

      if (this->show_debug_info_) {
         e->add(new DebugPart());
      } else {
         e->remove("debug");
      }
   }
}
