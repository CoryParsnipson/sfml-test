#include "../dependencies.h"

#include "Mouse.h"

Mouse::Mouse(sf::RenderWindow& window)
: window(window)
{
}

void Mouse::process_event(sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			std::cout << "Left Mouse Button down!" << std::endl;
		}
	}

	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			std::cout << "Left Mouse Button up!" << std::endl;
		}
	}

	if (event.type == sf::Event::MouseMoved) {
		if (!(this->get_last_mouse_position() == ViewportCoordinate(event.mouseMove.x, event.mouseMove.y))) {
			this->add_mouse_position(event.mouseMove.x, event.mouseMove.y);

			std::stringstream mouse_msg;
			mouse_msg << "Mouse move! (" << event.mouseMove.x << ", " << event.mouseMove.y << ")" << std::endl;
			std::cout << mouse_msg.str();
		}
	}
}

void Mouse::add_mouse_position(int x, int y) {
	this->last_mouse_positions.push_back(ViewportCoordinate(x, y));

	if (this->last_mouse_positions.size() > this->MOUSE_HISTORY_LENGTH) {
		this->last_mouse_positions.pop_front();
	}
}

ViewportCoordinate Mouse::get_last_mouse_position(int frames_to_go_back /* = 0 */) {
	if (this->last_mouse_positions.size() == 0) {
		return ViewportCoordinate(0, 0);
	}

	std::list<ViewportCoordinate>::iterator it = this->last_mouse_positions.end();
	for (int i = 0; i < frames_to_go_back, it != this->last_mouse_positions.begin(); --it, i++) {}

	return ViewportCoordinate(it->getX(), it->getY());
}