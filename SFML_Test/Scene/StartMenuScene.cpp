#include "StartMenuScene.h"
#include "BuilderScene.h"

#include "Game.h"

StartMenuScene::StartMenuScene()
: font_header_("retro", 36, FontConfig::ALIGN::CENTER)
, font_subtitle_("retro", 12, FontConfig::ALIGN::CENTER)
{
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg("StartMenuScene", Logger::INFO, "Entering game start menu state.");

   sf::Vector2f screen_size((float)Settings::Instance()->SCREEN_WIDTH, (float)Settings::Instance()->SCREEN_HEIGHT);
   
   // views
   this->viewports_["main"] = new Viewport(game.graphics, screen_size);
}

void StartMenuScene::exit(Game& game) {
}

void StartMenuScene::update(Game& game, Scene* scene, Entity* entity) {
   this->viewports_["main"]->write(
      "SFML TEST",
      this->viewports_["main"]->get_center(),
      &this->font_header_
   );

   this->viewports_["main"]->write(
      "main menu",
      this->viewports_["main"]->get_center() + sf::Vector2f(0.0, 45.0),
      &this->font_subtitle_
   );

   this->viewports_["main"]->write(
      "(Press SPACE or ENTER)",
      this->viewports_["main"]->get_center() + sf::Vector2f(0.0, 60.0),
      &this->font_subtitle_
   );
}

void StartMenuScene::process(Game& game, CloseCommand& c) {
   game.unload_scene();
}

void StartMenuScene::process(Game& game, KeyPressCommand& c) {
   if (c.event.code == sf::Keyboard::Key::Space || c.event.code == sf::Keyboard::Key::Return) {
      game.switch_scene(new BuilderScene());
   }
}

void StartMenuScene::process(Game& game, WindowResizeCommand& c) {
   sf::Vector2f screen_size((float)c.width, (float)c.height);
   sf::Vector2f screen_center(c.width / 2.f, c.height / 2.f);

   this->viewports_["main"]->set_size(screen_size);
   this->viewports_["main"]->set_center(screen_center);
}

void StartMenuScene::process(Game& game, MouseMoveCommand& c) {}
void StartMenuScene::process(Game& game, MouseButtonCommand& c) {}
void StartMenuScene::process(Game& game, MouseWheelCommand& c) {}
