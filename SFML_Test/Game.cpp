#include "Game.h"
#include "Entities/Map.h"
#include "Entities/Mouse.h"
#include "Util/InputController.h"

Game::Game()
   : HasState("Game")
   , GameEntity()
	, sw(window)
	, window(sf::VideoMode(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT), "SFML Test")
{
	window.setFramerateLimit(Settings::Instance()->FRAMERATE_LIMIT);
	window.setMouseCursorVisible(false);

	sw.load_font("retro", "retro.ttf");

   // set up states
   this->add_state("start_menu", [this]() { this->start_menu(); });
   this->add_state("map_menu", [this]() { this->map_menu(); });
   this->add_state("builder", [this]() { this->builder(); });

   this->add_transition("start_menu", "builder", [this](int input) {
      std::cout << "Testing start menu transition..." << std::endl;
      return (input == Game::INPUT::SPACE || input == Game::INPUT::ENTER);
   });

   this->add_transition("builder", "map_menu", [this](int input) {
      std::cout << "Testing map menu transition..." << std::endl;
      return (input == Game::INPUT::ESC);
   });
   
   // explicitly set init state
   this->set_init_state("start_menu");

   // set up input controller
   InputController& ic = InputController::instance();
   ic.registerInputListener(this);
}

Game::~Game() {
}

void Game::reset() {
   // game initialization
   
   // start game (will enter a game loop)
   this->on();
}

void Game::process_event() {

}

void Game::start_menu() {
   std::cout << "Entered start menu state." << std::endl;
   
   sf::Vector2f screen_middle(Settings::Instance()->SCREEN_WIDTH / (float)2, Settings::Instance()->SCREEN_HEIGHT / (float)2);
   sf::Vector2f screen_size((float)Settings::Instance()->SCREEN_WIDTH, (float)Settings::Instance()->SCREEN_HEIGHT);

   // game loop initialization 
   Game::INPUT key_pressed;
   
   // views
   sf::View view_main(screen_middle, screen_size);
  
   // entities

   // mess with screenwriter
   this->sw.set_alignment(ScreenWriter::ALIGNMENT::CENTER);
   
   // state loop
   while (this->window.isOpen()) {
      sf::Event event;
      key_pressed = Game::INPUT::NOP;

      InputController& ic = InputController::instance();
      ic.pollEvents(this->window);      

		while (this->window.pollEvent(event))
		{
			//if (event.type == sf::Event::Closed) {
			//	this->window.close();
			//}
			if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Return) {
               key_pressed = Game::INPUT::ENTER;
            }
            else if (event.key.code == sf::Keyboard::Key::Space) {
               key_pressed = Game::INPUT::SPACE;
            }
			}
     	}

      if (key_pressed != Game::INPUT::NOP) {
         // need to break out of this loop before we transition to another state
         // or we will have a memory leak...
         // this is kind of awkward...
         break;
      }

      // update window
      this->window.setView(view_main);
      this->window.clear();
      
      this->sw.set_font_size(36);
      this->sw.write("SFML TEST", static_cast<sf::Vector2i>(screen_middle));

      this->sw.set_font_size(12);
      this->sw.write("main menu", static_cast<sf::Vector2i>(screen_middle + sf::Vector2f(0.0, 45.0)));
      this->sw.write("(Press SPACE or ENTER)", static_cast<sf::Vector2i>(screen_middle + sf::Vector2f(0.0, 60.0)));

      this->window.display();
   }
   
   // this executes when the window is closed... Not good.
   this->update(key_pressed);
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
   sf::View view_main(screen_middle, screen_size);
   sf::View view_hud(screen_middle, screen_size); 

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

   Mouse m(this->window, view_main);

   // initialize "local" variables
   Game::INPUT key_pressed;
   sf::Vector2f original_view = view_main.getCenter();

   // fuck with shit
   sf::Mouse::setPosition(static_cast<sf::Vector2i>(screen_middle));
   this->sw.set_alignment(ScreenWriter::ALIGNMENT::LEFT);
   
   // state loop
   while (this->window.isOpen()) {
      sf::Event event;
      key_pressed = Game::INPUT::NOP;

		while (this->window.pollEvent(event))
		{
			//if (event.type == sf::Event::Closed)
			//{
			//	this->window.close();
			//}
			if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::R) {
               sf::Vector2f delta = original_view - view_main.getCenter();
               view_main.move(delta);
               
               // reset zoom too
               m.set_zoom_factor(1.0);
               view_main.setSize(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT);
            }
            else if (event.key.code == sf::Keyboard::Key::Escape) {
               key_pressed = Game::INPUT::ESC;
            }
			}
         else if (event.type == sf::Event::Resized) {
           	// scale views to new window size
				view_main.setSize((float)event.size.width, (float)event.size.height);
				view_hud.setSize((float)event.size.width, (float)event.size.height);

				// adjust position of views to new window size
				view_main.setCenter(event.size.width / 2.f, event.size.height / 2.f);
				view_hud.setCenter(event.size.width / 2.f, event.size.height / 2.f);
         }

         m.process_event(event);
     	}

      if (key_pressed != Game::INPUT::NOP) {
         // need to break out of this loop before we transition to another state
         // or we will have a memory leak...
         // this is kind of awkward...
         break;
      }

      // update window
      this->window.setView(view_main);
      this->window.clear();
      
      // draw map view items
      map.draw(this->window);
      m.draw(this->window, view_hud);

      // draw fixed hud items
      this->window.setView(view_hud);

		this->sw.write("SFML_Test");
		this->sw.write("r: reset pan position", sf::Vector2i(0, 15));
		this->sw.write("right click: click and drag to pan", sf::Vector2i(0, 30));

		sf::Vector2i mouse_pos = static_cast<sf::Vector2i>(m.get_cursor().getPosition());
		std::stringstream mmsg;
		mmsg << mouse_pos.x << ", " << mouse_pos.y;

		this->sw.write(mmsg.str(), mouse_pos + sf::Vector2i(0, 5));

      // draw to display
      this->window.display();
   }
   
   this->update(key_pressed);
}

void Game::process(CloseCommand& c) {
   this->window.close();
}
