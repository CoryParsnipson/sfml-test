#include "Game.h"
#include "Util/InputController.h"

#include "GameBuilderState.h"

// initialize static instance
GameBuilderState GameState::builder_state;

void GameBuilderState::enter(Game& game) {
   std::cout << "Entering builder start menu state." << std::endl;

   this->viewports["main"] = new Viewport(game, static_cast<sf::Vector2f>(game.window.getSize()));
   this->viewports["hud"] = new Viewport(game, static_cast<sf::Vector2f>(game.window.getSize()));

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

   game.m->set_view(this->viewports["main"]);
   sf::Mouse::setPosition(static_cast<sf::Vector2i>(this->viewports["main"]->get_center()));
   game.sw.set_alignment(ScreenWriter::ALIGNMENT::LEFT);
}

void GameBuilderState::exit(Game& game) {
   game.m->set_view(nullptr);
}

GameState* GameBuilderState::update(Game& game) {
   game.window.setView(this->viewports["main"]->get_view());
   game.window.clear();
   
   // draw map view items
   this->map->draw(game.window);
   game.m->draw(game.window, this->viewports["hud"]->get_view());

   // draw fixed hud items
   game.window.setView(this->viewports["hud"]->get_view());

	game.sw.write("SFML_Test");
	game.sw.write("r: reset pan position", sf::Vector2i(0, 15));
	game.sw.write("right click: click and drag to pan", sf::Vector2i(0, 30));

	sf::Vector2i mouse_pos = static_cast<sf::Vector2i>(game.m->get_cursor().getPosition());
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
   sf::Vector2f delta;

   switch (c.event.code) {
   case sf::Keyboard::Key::R:
      this->viewports["main"]->set_center(this->viewports["main"]->get_default_center());

      // reset zoom too
      this->viewports["main"]->set_size(this->viewports["main"]->get_default_size());
   break;
   default:
      // do nothing
   break;
   }
}

void GameBuilderState::process(Game& game, WindowResizeCommand& c) {
   std::map<std::string, Viewport*>::iterator it;
   
   sf::Vector2f screen_size((float)c.width, (float)c.height);
   sf::Vector2f screen_center(c.width / 2.f, c.height / 2.f);
   
   // keep hud viewport fixed over render window
   this->viewports["hud"]->set_size(screen_size);
   this->viewports["hud"]->set_center(screen_center);

   this->viewports["main"]->set_size(screen_size);
   this->viewports["main"]->set_default_size(screen_size);
}

void GameBuilderState::process(Game& game, MouseMoveCommand& c) {}
void GameBuilderState::process(Game& game, MouseButtonCommand& c) {}
void GameBuilderState::process(Game& game, MouseWheelCommand& c) {}
