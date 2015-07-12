#ifndef MOUSE_H
#define MOUSE_H

#include <list>

#include <SFML/Graphics.hpp>

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

	Mouse(sf::RenderWindow& window, sf::View& view);

	void process_event(sf::Event& event);
	void add_mouse_position(int x, int y);
	sf::Vector2i get_mouse_position();
	sf::Vector2i get_last_mouse_position(int frames_to_go_back = 0);

	sf::RectangleShape cursor;

protected:
	sf::RenderWindow& window;
	sf::View& view;

	std::list<sf::Vector2i> last_positions;	
};

#endif