#ifndef MOUSE_H
#define MOUSE_H

#include <SFML/Graphics.hpp>

#include "../Coordinate/ViewportCoordinate.h"
#include <list>

// ----------------------------------------------------------------------------
// class Mouse
// Wrapper class for SFML Mouse. Contains state information for click and drag
// etc.
// ----------------------------------------------------------------------------
class Mouse {
public:
	const unsigned int MOUSE_HISTORY_LENGTH = 10;

	Mouse(sf::RenderWindow& window);

	void process_event(sf::Event& event);
	void add_mouse_position(int x, int y);

	ViewportCoordinate get_last_mouse_position(int frames_to_go_back = 0);

protected:
	sf::RenderWindow& window;

	std::list<ViewportCoordinate> last_mouse_positions;
	
};

#endif