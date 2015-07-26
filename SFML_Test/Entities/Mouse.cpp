#include "Mouse.h"

Mouse::Mouse(sf::RenderWindow& window, sf::View& view)
: window(window)
, view(view)
, cursor(sf::Vector2f(6, 6))
, is_panning(false)
{
	cursor.setFillColor(sf::Color::Red);
	cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(this->window)));
}

void Mouse::process_event(sf::Event& event) {
	switch (event.type) {
	case sf::Event::Resized:
		break;
	case sf::Event::MouseButtonPressed:
		switch (event.mouseButton.button) {
		case sf::Mouse::Left:
			this->is_panning = true;
			this->panning_anchor = this->get_mouse_position();
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
			this->is_panning = false;
			this->panning_anchor = this->get_mouse_position();
			break;
		case sf::Mouse::Right:
			break;
		case sf::Mouse::Middle:
			break;
		}
		break;
	case sf::Event::MouseMoved:
		// update the red dot on the screen to the current position
		this->cursor.setPosition(static_cast<sf::Vector2f>(this->get_mouse_position()));
		if (!this->is_panning) {
			return;
		}
		sf::Vector2i pos = this->get_mouse_position();

		sf::Vector2f panning_delta = static_cast<sf::Vector2f>(this->panning_anchor - pos);
		this->view.move(this->MOUSE_PAN_COEFFICIENT * panning_delta);

		this->panning_anchor = pos;
		break;
	}
}

void Mouse::add_mouse_position(int x, int y) {
	this->last_positions.push_front(sf::Vector2i(x, y));

	if (this->last_positions.size() > this->MOUSE_HISTORY_LENGTH) {
		this->last_positions.pop_back();
	}
}

sf::Vector2i Mouse::get_mouse_position() {
	sf::Vector2i pos = sf::Mouse::getPosition(this->window);
	return sf::Vector2i(pos.x, pos.y);
}

sf::Vector2i Mouse::get_last_mouse_position(unsigned int frames_to_go_back /* = 0 */) {
	if (frames_to_go_back >= this->last_positions.size()) {
		return sf::Vector2i(0, 0);
	}

	std::list<sf::Vector2i>::iterator it = this->last_positions.begin();
	for (unsigned int i = 0; it != this->last_positions.end(), i < frames_to_go_back; it++, i++) {}

	return sf::Vector2i(it->x, it->y);
}

void Mouse::draw(sf::RenderWindow& window) {
	window.draw(this->cursor);
}

void Mouse::draw(sf::RenderWindow& window, sf::View& view) {
	sf::View old_view = window.getView();

	window.setView(view);
	this->draw(window);
	window.setView(old_view);
}