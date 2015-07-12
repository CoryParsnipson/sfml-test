#include "dependencies.h"

#include "Coordinate/ViewportCoordinate.h"
#include "Coordinate/MapCoordinate.h"
#include "FileReader/MapFileReader.h"
#include "Entities/Map.h"
#include "Entities/Mouse.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(Settings::Instance()->SCREEN_WIDTH, Settings::Instance()->SCREEN_HEIGHT), "SFML Test");
	window.setFramerateLimit(60);

	float screen_middle_x = Settings::Instance()->SCREEN_WIDTH / (float)2;
	float screen_middle_y = Settings::Instance()->SCREEN_HEIGHT / (float)2;

	// set view
	sf::View view(sf::Vector2f(screen_middle_x, screen_middle_y),
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
	sf::RectangleShape cursor(sf::Vector2f(6, 6));
	cursor.setPosition(sf::Vector2f(screen_middle_x, screen_middle_y) - cursor.getSize() / 2.f);
	cursor.setFillColor(sf::Color::Red);

	sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

	Mouse m(window);

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
			m.process_event(event);
		}

		// click and drag to pan map
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			sf::Vector2i mouse_new_position = sf::Mouse::getPosition(window);

			sf::Vector2i mouse_delta = mouse_new_position - mouse_position;

			view.move(static_cast<sf::Vector2f>(mouse_delta));
		}

// 		std::stringstream dbg_msg;
// 		dbg_msg << "Mouse Position: (" << sf::Mouse::getPosition().x << ", " << sf::Mouse::getPosition().y << ")\n";
// 		dbg_msg << "Cursor Position: (" << cursor.getPosition().x << ", " << cursor.getPosition().y << ")\n";
// 		std::cout << dbg_msg.str();

		// move view depending on mouse position
		//view.setCenter(static_cast<sf::Vector2f>(mouse_position));
		//view.move(static_cast<sf::Vector2f>(mouse_position) - view.getCenter());

		// mouse dependent calculations here
		cursor.setPosition(static_cast<sf::Vector2f>(mouse_position) - cursor.getSize() / 2.f);

		// attach your viewport to the window. This must be called every render cycle!
		window.setView(view);

		window.clear();

		// draw everything here
		map.draw(window);
		window.draw(cursor);

		// end the current frame
		window.display();

		mouse_position = sf::Mouse::getPosition(window);
	}

	return 0;
}