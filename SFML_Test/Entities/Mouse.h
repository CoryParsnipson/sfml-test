#ifndef MOUSE_H
#define MOUSE_H

#include "../dependencies.h"
#include "../Coordinate/ViewportCoordinate.h"

// ----------------------------------------------------------------------------
// class Mouse
// Wrapper class for SFML Mouse. Contains state information for click and drag
// etc.
// ----------------------------------------------------------------------------
class Mouse {
public:
	const unsigned int MOUSE_HISTORY_LENGTH = 10;
	const float MOUSE_PAN_COEFFICIENT = 0.15f;
	const float WINDOW_RESIZE_COEFFICIENT = 1.f;

	Mouse(sf::RenderWindow& window, sf::View& view);

	void process_event(sf::Event& event);
	void add_mouse_position(int x, int y);
	sf::Vector2i get_mouse_position();
	sf::Vector2i get_last_mouse_position(int frames_to_go_back = 0);

	void draw(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, sf::View& view);

protected:
	sf::RenderWindow& window;
	sf::View& view;
	sf::RectangleShape cursor;

	sf::Vector2u previous_window_size;
	sf::Vector2f window_resize_ratio;
	std::list<sf::Vector2i> last_positions;	
};

#endif