#include "../dependencies.h"

#include "Mouse.h"

Mouse::Mouse(sf::RenderWindow& window, sf::View& view)
: window(window)
, view(view)
, cursor(sf::Vector2f(6, 6))
{
	cursor.setFillColor(sf::Color::Red);
	//cursor.setOrigin()
	cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(this->window)));
}

void Mouse::process_event(sf::Event& event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left:

			break;
		case sf::Mouse::Right:
			break;
		case sf::Mouse::Middle:
			break;
		}
		break;
	case sf::Event::MouseButtonReleased:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left:
			break;
		case sf::Mouse::Right:
			break;
		case sf::Mouse::Middle:
			break;
		}
		break;
	case sf::Event::MouseMoved:
		if (!(this->get_last_mouse_position() == sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) {
			this->add_mouse_position(event.mouseMove.x, event.mouseMove.y);

			// mouse dependent calculations here
			cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				std::stringstream mouse_msg;
				mouse_msg << "Mouse move! (" << event.mouseMove.x << ", " << event.mouseMove.y << ")" << std::endl;
				std::cout << mouse_msg.str();

				// click and drag to pan map
				this->view.move(this->MOUSE_PAN_COEFFICIENT * static_cast<sf::Vector2f>(this->get_mouse_position() - this->get_last_mouse_position()));
			}
		}
		break;
	}
}

void Mouse::add_mouse_position(int x, int y) {
	this->last_positions.push_back(sf::Vector2i(x, y));

	if (this->last_positions.size() > this->MOUSE_HISTORY_LENGTH) {
		this->last_positions.pop_front();
	}
}

sf::Vector2i Mouse::get_mouse_position() {
	sf::Vector2i pos = sf::Mouse::getPosition(this->window);
	return sf::Vector2i(pos.x, pos.y);
}

sf::Vector2i Mouse::get_last_mouse_position(int frames_to_go_back /* = 0 */) {
	if (this->last_positions.size() == 0) {
		return sf::Vector2i(0, 0);
	}

	std::list<sf::Vector2i>::iterator it = this->last_positions.end();
	for (int i = 0; i < frames_to_go_back, it != this->last_positions.begin(); --it, i++) {}

	return sf::Vector2i(it->x, it->y);
}