#include "dependencies.h"

#include "Coordinate/ViewportCoordinate.h"
#include "Coordinate/MapCoordinate.h"
#include "FileReader/MapFileReader.h"
#include "Entities/Map.h"

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
	tile1->setSmooth(true);

	sf::Image tile2_nomask;
	tile2_nomask.loadFromFile("tile_grass.gif");
	tile2_nomask.createMaskFromColor(sf::Color::Magenta);

	sf::Texture* tile2 = new sf::Texture();
	tile2->loadFromImage(tile2_nomask);
	tile2->setSmooth(true);

	Map map;
	map.register_texture(tile1);
	map.register_texture(tile2);
	map.load_mapfile("map_test.txt");

	std::cout << "MAP INFO: " << std::endl << map.to_string() << std::endl;

	// set initial mouse position
	sf::Mouse::setPosition(sf::Vector2i(screen_middle_x, screen_middle_y), window);

	// main loop
	while (window.isOpen())
	{
		sf::Vector2i mouse_position = sf::Mouse::getPosition();

		// move view depending on mouse position
		//view.setCenter(static_cast<sf::Vector2f>(mouse_position));
		view.move(static_cast<sf::Vector2f>(mouse_position) - view.getCenter());

		// attach your viewport to the window. This must be called every render cycle!
		window.setView(view);

		// check for user clicking on x in upper right corner
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();

		// draw everything here
		map.draw(window);

		// end the current frame
		window.display();
	}

	return 0;
}