#include "Game.h"

#include "GameStartMenuState.h"
#include "GameBuilderState.h"

// initialize static instance
GameStartMenuState GameState::start_menu_state;

GameStartMenuState::GameStartMenuState()
: font_header("retro", 36, FontConfig::ALIGN::CENTER)
, font_subtitle("retro", 12, FontConfig::ALIGN::CENTER)
{
}

GameStartMenuState::~GameStartMenuState() {
   std::map<std::string, Viewport*>::const_iterator it;
   for (it = this->viewports.begin(); it != this->viewports.end(); it++) {
      delete it->second;
   }
}

void GameStartMenuState::enter(Game& game) {
   std::cout << "Entering game start menu state." << std::endl;

   sf::Vector2f screen_size((float)Settings::Instance()->SCREEN_WIDTH, (float)Settings::Instance()->SCREEN_HEIGHT);
   
   // views
   this->viewports["main"] = new Viewport(game.graphics, screen_size);
}

void GameStartMenuState::exit(Game& game) {
}

GameState* GameStartMenuState::update(Game& game) {
   this->viewports["main"]->write(
      "SFML TEST",
      this->viewports["main"]->get_center(),
      &this->font_header
   );

   this->viewports["main"]->write(
      "main menu",
      this->viewports["main"]->get_center() + sf::Vector2f(0.0, 45.0),
      &this->font_subtitle
   );

   this->viewports["main"]->write(
      "(Press SPACE or ENTER)",
      this->viewports["main"]->get_center() + sf::Vector2f(0.0, 60.0),
      &this->font_subtitle
   );

   return this->next_state_;
}

void GameStartMenuState::process(Game& game, CloseCommand& c) {
   std::cout << "[GameStartMenuState] Close Command" << std::endl;
   
   game.exit();
}

void GameStartMenuState::process(Game& game, KeyPressCommand& c) {
   std::cout << "[GameStartMenuState] KeyPress Command" << std::endl;

   if (c.event.code == sf::Keyboard::Key::Space || c.event.code == sf::Keyboard::Key::Return) {
      this->next_state_ = &GameState::builder_state;
   }
}

void GameStartMenuState::process(Game& game, WindowResizeCommand& c) {
   std::cout << "[GameStartMenuState] WindowResize Command" << std::endl;

   sf::Vector2f screen_size((float)c.width, (float)c.height);
   sf::Vector2f screen_center(c.width / 2.f, c.height / 2.f);

   this->viewports["main"]->set_size(screen_size);
   this->viewports["main"]->set_center(screen_center);
}

void GameStartMenuState::process(Game& game, MouseMoveCommand& c) {
   std::cout << "[GameStartMenuState] MouseMove Command" << std::endl;
}

void GameStartMenuState::process(Game& game, MouseButtonCommand& c) {
   std::cout << "[GameStartMenuState] MouseButton Command" << std::endl;
}

void GameStartMenuState::process(Game& game, MouseWheelCommand& c) {
   std::cout << "[GameStartMenuState] MouseWheel Command" << std::endl;
}
