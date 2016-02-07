#include "StartMenuScene.h"
#include "BuilderScene.h"

#include "Game.h"
#include "Graphics.h"
#include "Layer.h"
#include "TextFactory.h"
#include "GraphicsPart.h"

StartMenuScene::StartMenuScene()
{
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg("StartMenuScene", Logger::INFO, "Entering game start menu state.");

   // create viewport(s)
   this->viewport_ = new Viewport(sf::Vector2f(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT));

   // populate entities
   Entity* title_text = TextFactory::inst()->create_text(
      "SFML TEST",
      "retro",
      this->viewport_->layer("main")->get_center(),
      36,
      TextFactory::ALIGN::CENTER
   );

   Entity* subtitle_text1 = TextFactory::inst()->create_text(
      "main menu",
      "retro",
      this->viewport_->layer("main")->get_center() + sf::Vector2f(0, 45),
      12,
      TextFactory::ALIGN::CENTER
   );

   Entity* subtitle_text2 = TextFactory::inst()->create_text(
      "(Press SPACE or ENTER)",
      "retro",
      this->viewport_->layer("main")->get_center() + sf::Vector2f(0, 60),
      12,
      TextFactory::ALIGN::CENTER
   );

   // add to update list
   this->entities_.push_back(title_text);
   this->entities_.push_back(subtitle_text1);
   this->entities_.push_back(subtitle_text2);

   // add to draw list
   this->viewport_->layer("main")->add(title_text);
   this->viewport_->layer("main")->add(subtitle_text1);
   this->viewport_->layer("main")->add(subtitle_text2);
}

void StartMenuScene::exit(Game& game) {
   Service::get_logger().msg("StartMenuScene", Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);
}

void StartMenuScene::process(Game& game, CloseCommand& c) {
   game.unload_scene();
}

void StartMenuScene::process(Game& game, KeyPressCommand& c) {
   if (c.event.code == sf::Keyboard::Key::Space || c.event.code == sf::Keyboard::Key::Return) {
      game.switch_scene(new BuilderScene());
   } else if (c.event.code == sf::Keyboard::Key::B) {
      EntityList::const_iterator it;
      for (it = this->entities_.begin(); it != this->entities_.end(); ++it) {
         GraphicsPart* graphics_part = dynamic_cast<GraphicsPart*>((*it)->get("graphics"));
         graphics_part->set_show_debug_text(!graphics_part->get_show_debug_text());
      }
   }
}

void StartMenuScene::process(Game& game, WindowResizeCommand& c) {
   this->viewport_->resize(sf::Vector2f(c.width, c.height));
   this->viewport_->recenter(sf::Vector2f(c.width / 2.f, c.height / 2.f));
}

void StartMenuScene::process(Game& game, MouseMoveCommand& c) {}
void StartMenuScene::process(Game& game, MouseButtonCommand& c) {}
void StartMenuScene::process(Game& game, MouseWheelCommand& c) {}
