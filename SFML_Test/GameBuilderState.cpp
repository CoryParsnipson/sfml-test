#include "Game.h"
#include "Util/InputController.h"

#include "GameBuilderState.h"

// initialize static instance
GameBuilderState GameState::builder_state;

void GameBuilderState::enter(Game& game) {
   std::cout << "Entering builder start menu state." << std::endl;

   this->screen_middle.x = Settings::Instance()->SCREEN_WIDTH / (float)2;
   this->screen_middle.y = Settings::Instance()->SCREEN_HEIGHT / (float)2;

   this->screen_size.x = (float)Settings::Instance()->SCREEN_WIDTH; 
   this->screen_size.y = (float)Settings::Instance()->SCREEN_HEIGHT;

   this->views["view_main"] = new sf::View(screen_middle, screen_size);
   this->views["view_hud"] = new sf::View(screen_middle, screen_size);

   this->origin = this->views["view_main"]->getCenter();

   // load tile textures
   this->tile1_nomask = new sf::Image();
   this->tile1_nomask->loadFromFile("tile1.gif");
   this->tile1_nomask->createMaskFromColor(sf::Color::Magenta);

   this->tile1 = new sf::Texture();
   this->tile1->loadFromImage(*this->tile1_nomask);

   this->tile2_nomask = new sf::Image();
	this->tile2_nomask->loadFromFile("tile_grass.gif");
	this->tile2_nomask->createMaskFromColor(sf::Color::Magenta);

   this->tile2 = new sf::Texture();
   this->tile2->loadFromImage(*this->tile2_nomask);

   // entities
   this->map = new Map();
   this->map->register_texture(this->tile1);
   this->map->register_texture(this->tile2);
   this->map->load_mapfile("map_test.txt");

   this->m = new Mouse(game.window, *(this->views["view_main"]));

   InputController& ic = InputController::instance();
   ic.registerInputListener(this->m);

   sf::Mouse::setPosition(static_cast<sf::Vector2i>(screen_middle));
   game.sw.set_alignment(ScreenWriter::ALIGNMENT::LEFT);
}

void GameBuilderState::exit(Game& game) {
}

GameState* GameBuilderState::update(Game& game) {
   game.window.setView(*(this->views["view_main"]));
   game.window.clear();
   
   // draw map view items
   this->map->draw(game.window);
   this->m->draw(game.window, *(this->views["view_hud"]));

   // draw fixed hud items
   game.window.setView(*(this->views["view_hud"]));

	game.sw.write("SFML_Test");
	game.sw.write("r: reset pan position", sf::Vector2i(0, 15));
	game.sw.write("right click: click and drag to pan", sf::Vector2i(0, 30));

	sf::Vector2i mouse_pos = static_cast<sf::Vector2i>(this->m->get_cursor().getPosition());
	std::stringstream mmsg;
	mmsg << mouse_pos.x << ", " << mouse_pos.y;

	game.sw.write(mmsg.str(), mouse_pos + sf::Vector2i(0, 5));

   // draw to display
   game.window.display();

   return NULL;
}

void GameBuilderState::process(Game& game, CloseCommand& c) {
   std::cout << "[GameBuilderState] Close Command" << std::endl;
   game.exit();
}

void GameBuilderState::process(Game& game, KeyPressCommand& c) {
   if (c.event.code == sf::Keyboard::Key::R) {
      sf::Vector2f delta = this->origin - this->views["view_main"]->getCenter();
      this->views["view_main"]->move(delta);

      // reset zoom too
      this->views["view_main"]->setSize(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT);
   }
}

void GameBuilderState::process(Game& game, WindowResizeCommand& c) {
   std::map<std::string, sf::View*>::iterator it;

   for (it = this->views.begin(); it != this->views.end(); it++) {
      it->second->setSize((float)c.width, (float)c.height);
      it->second->setCenter(c.width / 2.f, c.height / 2.f);
   };
}

void GameBuilderState::process(Game& game, MouseMoveCommand& c) {
}

void GameBuilderState::process(Game& game, MouseButtonCommand& c) {
}

void GameBuilderState::process(Game& game, MouseWheelCommand& c) {
}
