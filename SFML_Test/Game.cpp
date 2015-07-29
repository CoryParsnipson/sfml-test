#include "Game.h"
#include "Entities/Map.h"
#include "Entities/Mouse.h"

Game::Game()
	: window(sf::VideoMode(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT), "SFML Test")
	, sw(window)
{
	window.setFramerateLimit(Settings::Instance()->FRAMERATE_LIMIT);
	window.setMouseCursorVisible(false);

	sw.load_font("retro", "retro.ttf");

	// enter game loop
	this->loop();
}

void Game::loop() {
	float screen_middle_x = Settings::Instance()->SCREEN_WIDTH / (float)2;
	float screen_middle_y = Settings::Instance()->SCREEN_HEIGHT / (float)2;

	// set view
	sf::View view_map(sf::Vector2f(screen_middle_x, screen_middle_y),
		sf::Vector2f((float)Settings::Instance()->SCREEN_WIDTH, (float)Settings::Instance()->SCREEN_HEIGHT));

	sf::View view_fixed(sf::Vector2f(screen_middle_x, screen_middle_y),
		sf::Vector2f((float)Settings::Instance()->SCREEN_WIDTH, (float)Settings::Instance()->SCREEN_HEIGHT));

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

	Map map;
	map.register_texture(tile1);
	map.register_texture(tile2);
	map.load_mapfile("map_test.txt");

	// set initial mouse position
	sf::Mouse::setPosition(sf::Vector2i((int)screen_middle_x, (int)screen_middle_y), this->window);
	sf::Vector2i mouse_position = sf::Mouse::getPosition(this->window);

	Mouse m(this->window, view_map);

	// get starting view position
	sf::Vector2f original_view = view_map.getCenter();

	// NOTE: window.mapPixelToCoords(const sf::Vector2i&, const sf::View&) -> use to make a sprite
	// fixed against computer monitor (instead of using a second view)

	// NOTE to the NOTE: This function does not preclude the usefulness of having
	// multiple views. You might still want a separate view for user HUD just for
	// semantics or convenience.

	// main loop
	while (this->window.isOpen())
	{
		sf::Event event;
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				this->window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Key::R) {
					sf::Vector2f delta = original_view - view_map.getCenter();
					view_map.move(delta);
				}
			}
			else if (event.type == sf::Event::Resized) {
				// scale views to new window size
				view_map.setSize((float)event.size.width, (float)event.size.height);
				view_fixed.setSize((float)event.size.width, (float)event.size.height);

				// adjust position of views to new window size
				view_map.setCenter(event.size.width / 2.f, event.size.height / 2.f);
				view_fixed.setCenter(event.size.width / 2.f, event.size.height / 2.f);
			}

			m.process_event(event);
		}

		// attach your viewport to the window. This must be called every render cycle!
		this->window.setView(view_map);

		this->window.clear();

		// draw everything here
		map.draw(this->window);
		m.draw(this->window, view_fixed);

		// attach to second view that is fixed wrt computer monitor
		this->window.setView(view_fixed);

		// draw basic hud instructions
		this->sw.write("SFML_Test");
		this->sw.write("r: reset pan position", sf::Vector2i(0, 15));
		this->sw.write("right click: click and drag to pan", sf::Vector2i(0, 30));

		//sf::Vector2i mouse_pos = m.get_mouse_position();
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(this->window);
		std::stringstream mmsg;
		mmsg << mouse_pos.x << ", " << mouse_pos.y;

		this->sw.write(mmsg.str(), mouse_pos + sf::Vector2i(0, 5));

		// end the current frame
		this->window.display();

		mouse_position = sf::Mouse::getPosition(this->window);
	}
}

void Game::process_event() {

}