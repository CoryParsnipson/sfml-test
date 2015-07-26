#include "dependencies.h"

#include "Coordinate/ViewportCoordinate.h"
#include "Coordinate/MapCoordinate.h"
#include "FileReader/MapFileReader.h"
#include "Entities/Map.h"
#include "Entities/Mouse.h"
#include "Entities/ScreenWriter.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT), "SFML Test");
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

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
	sf::Mouse::setPosition(sf::Vector2i((int)screen_middle_x, (int)screen_middle_y), window);
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

	Mouse m(window, view_map);
	ScreenWriter sw(window);

	sw.load_font("retro", "retro.ttf");

	// get starting view position
	sf::Vector2f original_view = view_map.getCenter();

	// NOTE: window.mapPixelToCoords(const sf::Vector2i&, const sf::View&) -> use to make a sprite
	// fixed against computer monitor (instead of using a second view)

	// NOTE to the NOTE: This function does not preclude the usefulness of having
	// multiple views. You might still want a separate view for user HUD just for
	// semantics or convenience.

	// main loop
	while (window.isOpen())
	{
		// iterate through Window event queue
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
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
		window.setView(view_map);

		window.clear();

		// draw everything here
		map.draw(window);
		m.draw(window, view_fixed);
		
		// attach to second view that is fixed wrt computer monitor
		window.setView(view_fixed);
		
		// draw basic hud instructions
		sw.write("SFML_Test");
		sw.write("r: reset pan position", sf::Vector2i(0, 15));
		sw.write("middle click: click and drag to pan", sf::Vector2i(0, 30));

		//sf::Vector2i mouse_pos = m.get_mouse_position();
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
		std::stringstream mmsg;
		mmsg << mouse_pos.x << ", " << mouse_pos.y;

		sw.write(mmsg.str(), mouse_pos + sf::Vector2i(0, 5));

		// end the current frame
		window.display();

		mouse_position = sf::Mouse::getPosition(window);
	}

	return 0;
}