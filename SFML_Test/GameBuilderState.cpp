#include "Game.h"
#include "Util/InputController.h"

#include "Util/Graphics.h"
#include "Util/TileGraphicsComponent.h"

#include "GameBuilderState.h"

// initialize static instance
GameBuilderState GameState::builder_state;

void GameBuilderState::enter(Game& game) {
   std::cout << "Entering builder start menu state." << std::endl;

   this->viewports["main"] = new Viewport(game, static_cast<sf::Vector2f>(game.graphics.get_window().getSize()));
   this->viewports["hud"] = new Viewport(game, static_cast<sf::Vector2f>(game.graphics.get_window().getSize()));

   // load textures
   game.texture_manager.create_texture(0, "tile1.gif");
   game.texture_manager.create_texture(1, "tile_grass.gif");

   // entities
   this->map = new Map();
   this->map->load_mapfile(game, "map_test.txt");

   std::cout << game.texture_manager.to_string() << std::endl;

   game.m->set_view(this->viewports["main"]);
   sf::Mouse::setPosition(static_cast<sf::Vector2i>(this->viewports["main"]->get_center()));
   game.sw.set_alignment(ScreenWriter::ALIGNMENT::LEFT);
}

void GameBuilderState::exit(Game& game) {
   game.m->set_view(nullptr);
}

GameState* GameBuilderState::update(Game& game) {
   game.graphics.clear();

   // draw map view items
   this->map->draw(game.graphics);
   game.m->draw(game.graphics, this->viewports["hud"]);

   // draw fixed hud items
	game.sw.write(game.graphics, *this->viewports["hud"], "SFML_Test");
	game.sw.write(game.graphics, *this->viewports["hud"], "r: reset pan position", sf::Vector2i(0, 15));
	game.sw.write(game.graphics, *this->viewports["hud"], "right click: click and drag to pan", sf::Vector2i(0, 30));

	sf::Vector2i mouse_pos = static_cast<sf::Vector2i>(game.m->get_cursor().getPosition());
	std::stringstream mmsg;
	mmsg << mouse_pos.x << ", " << mouse_pos.y;

	game.sw.write(game.graphics, *this->viewports["hud"], mmsg.str(), mouse_pos + sf::Vector2i(0, 5));

   // draw to display
   game.graphics.update();

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
