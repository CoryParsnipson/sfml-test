#include "Game.h"
#include "Entities/Map.h"
#include "Util/InputController.h"

#include "GameStartMenuState.h"

Game::Game()
   : InputListener()
	, sw(this->window)
	, window(sf::VideoMode(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT), "SFML Test")
   , is_running(true)
{
	window.setFramerateLimit(Settings::Instance()->FRAMERATE_LIMIT);
	window.setMouseCursorVisible(false);

	sw.load_font("retro", "retro.ttf");

   // set up input controller
   InputController& ic = InputController::instance();
   ic.registerInputListener(this);
}

Game::~Game() {
}

void Game::reset() {
   // game initialization
   this->is_running = true;
   
   // specify explicit start state
   delete this->state_;
   this->state_ = &GameState::start_menu_state;

   // start game loop
   this->main_loop();
}

void Game::main_loop() {
   while (this->window.isOpen()) {
      GameState* state = this->state_->update(*this);
      if (state != NULL) {
         this->state_->exit(*this);

         delete this->state_;
         this->state_ = state;

         this->state_->enter(*this);
      }
   }
}

void Game::start_menu() {
   std::cout << "Entered start menu state." << std::endl;
   
   sf::Vector2f screen_middle(Settings::Instance()->SCREEN_WIDTH / (float)2, Settings::Instance()->SCREEN_HEIGHT / (float)2);
   sf::Vector2f screen_size((float)Settings::Instance()->SCREEN_WIDTH, (float)Settings::Instance()->SCREEN_HEIGHT);

   // views
   this->views["view_main"] = new sf::View(screen_middle, screen_size);

   // mess with screenwriter
   this->sw.set_alignment(ScreenWriter::ALIGNMENT::CENTER);
   
   // state loop
   while (this->window.isOpen()) {
      InputController& ic = InputController::instance();
      ic.pollEvents(this->window);      
   
      // check for exit condition
      if (!this->is_running) {
         std::cout << "Exiting from start menu state!" << std::endl;
         break;
      }

      // update window
      this->window.setView(*(this->views["view_main"]));
      this->window.clear();
      
      this->sw.set_font_size(36);
      this->sw.write("SFML TEST", static_cast<sf::Vector2i>(screen_middle));

      this->sw.set_font_size(12);
      this->sw.write("main menu", static_cast<sf::Vector2i>(screen_middle + sf::Vector2f(0.0, 45.0)));
      this->sw.write("(Press SPACE or ENTER)", static_cast<sf::Vector2i>(screen_middle + sf::Vector2f(0.0, 60.0)));

      this->window.display();
   }
}

void Game::map_menu() {
   std::cout << "Entered map menu state." << std::endl;

   throw "Not implemented yet...";
}

void Game::builder() {
   std::cout << "Entered builder state." << std::endl;

   sf::Vector2f screen_middle(Settings::Instance()->SCREEN_WIDTH / (float)2, Settings::Instance()->SCREEN_HEIGHT / (float)2);
   sf::Vector2f screen_size((float)Settings::Instance()->SCREEN_WIDTH, (float)Settings::Instance()->SCREEN_HEIGHT);

   // -- game loop initialization 
   
   // views
   this->views["view_main"] = new sf::View(screen_middle, screen_size);
   this->views["view_hud"] = new sf::View(screen_middle, screen_size);

   // load tile textures
   sf::Image tile1_nomask;
   tile1_nomask.loadFromFile("tile1.gif");
   tile1_nomask.createMaskFromColor(sf::Color::Magenta);

   sf::Texture* tile1 = new sf::Texture();
   tile1->loadFromImage(tile1_nomask);

	sf::Image tile2_nomask;
	tile2_nomask.loadFromFile("tile_grass.gif");
	tile2_nomask.createMaskFromColor(sf::Color::Magenta);

   sf::Texture* tile2 = new sf::Texture();
   tile2->loadFromImage(tile2_nomask);

   // entities
   Map map;
   map.register_texture(tile1);
   map.register_texture(tile2);
   map.load_mapfile("map_test.txt");

   this->m = new Mouse(this->window, *(this->views["view_main"]));
   
   // register mouse listener with input controller
   InputController& ic = InputController::instance();
   ic.registerInputListener(this->m);

   // initialize "local" variables
   this->origin = this->views["view_main"]->getCenter();

   // fuck with shit
   sf::Mouse::setPosition(static_cast<sf::Vector2i>(screen_middle));
   this->sw.set_alignment(ScreenWriter::ALIGNMENT::LEFT);
   
   // state loop
   while (this->window.isOpen()) {
      InputController& ic = InputController::instance();
      ic.pollEvents(this->window);

      // check for exit condition
      if (!this->is_running) {
         std::cout << "Exiting from builder state!" << std::endl;
         break;
      }

      // update window
      this->window.setView(*(this->views["view_main"]));
      this->window.clear();
      
      // draw map view items
      map.draw(this->window);
      this->m->draw(this->window, *(this->views["view_hud"]));

      // draw fixed hud items
      this->window.setView(*(this->views["view_hud"]));

		this->sw.write("SFML_Test");
		this->sw.write("r: reset pan position", sf::Vector2i(0, 15));
		this->sw.write("right click: click and drag to pan", sf::Vector2i(0, 30));

		sf::Vector2i mouse_pos = static_cast<sf::Vector2i>(this->m->get_cursor().getPosition());
		std::stringstream mmsg;
		mmsg << mouse_pos.x << ", " << mouse_pos.y;

		this->sw.write(mmsg.str(), mouse_pos + sf::Vector2i(0, 5));

      // draw to display
      this->window.display();
   }
}

void Game::exit() {
   this->is_running = false;
   this->window.close();
}

void Game::process(CloseCommand& c) {
   this->state_->process(c);
}

void Game::process(KeyPressCommand& c) {
   this->state_->process(c);
}

void Game::process(WindowResizeCommand& c) {
   this->state_->process(c);
}

void Game::process(MouseMoveCommand& c) {
   this->state_->process(c);
}

void Game::process(MouseButtonCommand& c) {
   this->state_->process(c);
}

void Game::process(MouseWheelCommand& c) {
   this->state_->process(c);
}

//void Game::process(CloseCommand& c) {
//   this->exit();
//}
//
//void Game::process(KeyPressCommand& c) {
//   std::cout << "Game::process[KeyPressCommand&] -> received keycode '" << c.event.code << "'" << std::endl;
//   
//   switch (c.event.code) {
//   case sf::Keyboard::Key::Return:
//      this->update(Game::INPUT::ENTER);
//      break;  
//   case sf::Keyboard::Key::Escape:
//      this->update(Game::INPUT::ESC);
//      break;
//   case sf::Keyboard::Key::R:
//      // want to find some way of making this code less clunky
//      if (std::string("builder") == this->get_current_state()) {
//         // center view to new window size
//         sf::Vector2f delta = this->origin - this->views["view_main"]->getCenter();
//         this->views["view_main"]->move(delta);
//
//         // reset zoom too
//         this->views["view_main"]->setSize(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT);
//      }
//      break;
//   default:
//      return;
//   }
//}
//
//void Game::process(WindowResizeCommand& c) {
//   std::map<std::string, sf::View*>::iterator it;
//
//   for (it = this->views.begin(); it != this->views.end(); it++) {
//      it->second->setSize((float)c.width, (float)c.height);
//      it->second->setCenter(c.width / 2.f, c.height / 2.f);
//   };
//};
//
//void Game::process(MouseMoveCommand& c) { } // ignore
//void Game::process(MouseButtonCommand& c) { } // ignore
//void Game::process(MouseWheelCommand& c) { } // ignore
