#include "Game.h"

#include "GameStartMenuState.h"
#include "GameBuilderState.h"

// initialize static instance
GameStartMenuState GameState::start_menu_state;

void GameStartMenuState::enter(Game& game) {
   std::cout << "Entering game start menu state." << std::endl;

   this->screen_middle.x = Settings::Instance()->SCREEN_WIDTH / (float)2;
   this->screen_middle.y = Settings::Instance()->SCREEN_HEIGHT / (float)2; 
   
   this->screen_size.x = (float)Settings::Instance()->SCREEN_WIDTH;
   this->screen_size.y = (float)Settings::Instance()->SCREEN_HEIGHT;

   // views
   this->views["view_main"] = new sf::View(this->screen_middle, this->screen_size);

   // mess with screenwriter
   game.sw.set_alignment(ScreenWriter::ALIGNMENT::CENTER);
}

void GameStartMenuState::exit(Game& game) {
}

GameState* GameStartMenuState::update(Game& game) {
   // update window
   game.window.setView(*(this->views["view_main"]));
   game.window.clear();

   game.sw.set_font_size(36);
   game.sw.write("SFML TEST", static_cast<sf::Vector2i>(this->screen_middle));

   game.sw.set_font_size(12);
   game.sw.write("main menu", static_cast<sf::Vector2i>(this->screen_middle + sf::Vector2f(0.0, 45.0)));
   game.sw.write("(Press SPACE or ENTER)", static_cast<sf::Vector2i>(this->screen_middle + sf::Vector2f(0.0, 60.0)));

   game.window.display();

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
   
   this->screen_size.x = (float)c.width;
   this->screen_size.y = (float)c.height;

   this->screen_middle.x = c.width / 2.f;
   this->screen_middle.y = c.height / 2.f;
   
   this->views["view_main"]->setSize(screen_size);
   this->views["view_main"]->setCenter(screen_middle);
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
