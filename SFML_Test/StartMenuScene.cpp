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
   this->camera_->move(-1.f * sf::Vector2f(Settings::Instance()->cur_width() / 2.f, Settings::Instance()->cur_height() / 2.f));

   // populate entities
   SceneObject* node = TextFactory::inst()->create_text_entity(
      "SFML TEST",
      "retro",
      sf::Vector2f(0, 0),
      36,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "main menu",
      "retro",
      sf::Vector2f(0, 45),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   );
   this->scene_graph_->add(node);

   node = TextFactory::inst()->create_text_entity(
      "(Press SPACE or ENTER)",
      "retro",
      sf::Vector2f(0, 60),
      12,
      TextFactory::ALIGN::CENTER,
      sf::Color::White,
      this->show_debug_info_
   );
   this->scene_graph_->add(node);

   this->gamepad(new PlayerGamepad());
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering game start menu state.");
}

void StartMenuScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::process(Game& game, ResizeInputEvent& e) {
   sf::Vector2f original_offset(this->camera_->get_size() / 2.f);

   Scene::process(game, e);

   sf::Vector2f new_center(e.width / 2.f, e.height / 2.f);

   this->camera_->set_size(sf::Vector2f(e.width, e.height));
   this->camera_->move(original_offset - new_center);
}

void StartMenuScene::process(Game& game, KeyPressInputEvent& e) {
   if (e.key == Key::Space || e.key == Key::Return) {
      game.load_scene(new BuilderScene());
   } else if (e.key == Key::Escape) {
      // go to super special awesome test scene
      game.load_scene(new TestUIScene());
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
